#include <freertos/FreeRTOS.h>
#include <freertos/ringbuf.h>

#include "esp32-hal-uart.h"
#include "esp32-hal.h"

#include "LoggingService.hpp"

RingbufHandle_t log_ringbuffer = NULL;

// TODO - Automate this monkeypatching or find another solution
// C:\Users\Avery\.platformio\packages\framework-arduinoespressif32\cores\esp32\esp32-hal-uart.c
// int log_printfv(const char *format, va_list arg)
// int __attribute__((weak)) log_printfv(const char *format, va_list arg)
// Arduino logging is handled via log_printfv being a weak symbol
// TODO - Doesn't handle recursion, etc detection
void backup_vprintf(int count, ...) {
  va_list args;
  va_start(args, count);
  vprintf("!!%s", args);
  va_end(args);
}

void extra_vprintf(int count, ...) {
  va_list args;
  va_start(args, count);
  vprintf("||%s", args);
  va_end(args);
}

#ifdef LOGGING_HOOK_ENABLED
extern "C" {
  int log_printfv(const char *format, va_list arg)
  {
    if (!format)
      return 0;

    // Changed from default of 64 -> 85 to handle 80 column width terminal messages
    static char loc_buf[85];
    char *temp = loc_buf;
    uint32_t len;
    va_list copy;
    va_list copy2;
    va_copy(copy, arg);
    va_copy(copy2, arg);
    len = vsnprintf(NULL, 0, format, copy);
    va_end(copy);
    if (len >= sizeof(loc_buf))
    {
      temp = (char *)malloc(len + 1);
      if (temp == NULL)
      {
        return 0;
      }
    }

    vsnprintf(temp, len + 1, format, arg);

    bool success = pdFALSE;
    if (log_ringbuffer != NULL) {
      success = xRingbufferSend(log_ringbuffer, temp, len + 1, pdMS_TO_TICKS(0));
    }

    if (success == pdFALSE) {
      backup_vprintf(1, temp);
    }

    if (len >= sizeof(loc_buf))
    {
      free(temp);
    }
    return len;
  }
}
#endif

LoggingStateService::LoggingStateService(AsyncWebServer *server, SecurityManager *securityManager) : _httpEndpoint(
                                                                                                              LoggingState::read,
                                                                                                              LoggingState::update,
                                                                                                              this,
                                                                                                              server,
                                                                                                              LOGGING_ENDPOINT_PATH,
                                                                                                              securityManager,
                                                                                                              AuthenticationPredicates::IS_AUTHENTICATED),
                                                                                                          _webSocket(
                                                                                                              LOGGING_SOCKET_PATH)
{


  log_i("Creating Logging Service");

  // TODO - Create ring buffer in PSRAM if possible. Allows higher log buffer amount?
  log_ringbuffer = xRingbufferCreate(256 * 64, RINGBUF_TYPE_NOSPLIT);
  if (log_ringbuffer == NULL)
  {
    ESP_LOGE("logging", "Failed to create ring buffer. Aborting hook setup");
  } else {
    #ifdef LOGGING_HOOK_ENABLED
    log_i("Hooking ESP LOG!");
    esp_log_set_vprintf(&log_printfv);
    #endif
  }

  addUpdateHandler([&](const String &originId)
                   { onConfigUpdated(); },
                   false);

  _webSocket.setFilter(
      securityManager->filterRequest(AuthenticationPredicates::IS_AUTHENTICATED));
  _webSocket.onEvent(std::bind(&LoggingStateService::onWSEvent,
                               this,
                               std::placeholders::_1,
                               std::placeholders::_2,
                               std::placeholders::_3,
                               std::placeholders::_4,
                               std::placeholders::_5,
                               std::placeholders::_6));
  server->addHandler(&_webSocket);
  server->on(
      LOGGING_SOCKET_PATH, HTTP_GET,
      std::bind(&LoggingStateService::forbidden, this, std::placeholders::_1));

  log_i("Finished setup of Logging Service");
}


void LoggingStateService::begin()
{
  xTaskCreatePinnedToCore(
      &LoggingStateService::taskSendLogs,
      "logging_task", 4096, this, tskIDLE_PRIORITY, NULL, 1);
}

void LoggingStateService::onWSEvent(
    AsyncWebSocket *server,
    AsyncWebSocketClient *client,
    AwsEventType type,
    void *arg,
    uint8_t *data,
    size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    this->client = client;
  }

  if (type == WS_EVT_DISCONNECT)
  {
    this->client = nullptr;
  }
}

void LoggingStateService::taskSendLogs(void *parameter)
{
  LoggingStateService *service = (LoggingStateService *)parameter;

  size_t size;
  uint8_t *message;

  while (true)
  {
    if (log_ringbuffer == NULL || service->client == nullptr)
    {
      vTaskDelay(10 / portTICK_PERIOD_MS);
      continue;
    }

    message = reinterpret_cast<uint8_t *>(xRingbufferReceive(log_ringbuffer, &size, 100 / portTICK_PERIOD_MS));
    if (message == nullptr)
    {
      continue;
    }

    AsyncWebSocketMessageBuffer *buffer = service->_webSocket.makeBuffer(message, size);
    extra_vprintf(1, message); // TODO - TEMP until logging is confirmed working

    vRingbufferReturnItem(log_ringbuffer, message);

    service->_webSocket.textAll(buffer);
  }
}