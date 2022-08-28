#include "data_logger.hpp"
#include <ArduinoJson.h>

const char* WebLogLevel_KEYS[WebLogLevel_NUM_OF_ITEMS] = {
  "verbose", "debug", "info", "warning", "error"
};

const char* DataParameter_NAMES[DataParameter_NUM_OF_ITEMS] = {
  "Actual Position",
  "Actual Velocity",
  "Actual Force",

  "Demand Position",
  "Demand Velocity",

  "Motor Supply Voltage",
  "Motor Power Losses",
  "Model Temp",
  "Real Temp"
};
const char* DataParameter_KEYS[DataParameter_NUM_OF_ITEMS] = {
  "apos",
  "avel",
  "aforce",

  "dpos",
  "dvel",

  "vsp",
  "wloss",
  "mtemp",
  "rtemp"
};

WebLogger wlog;

void WebLogger::startTask() {
  xTaskCreate(&WebLogger::sendData, "weblogger_send_data", 4096, this, 5, &this->sendDataHandle);
}

void WebLogger::log(DataParameter key, float value) {
  if (xSemaphoreTake(this->datapoints_writeLock, portMAX_DELAY) != pdTRUE) {
    // TODO - Log error
    return;
  }

  int64_t now = esp_timer_get_time();

  uint8_t keyIndex = static_cast<uint8_t>(key);
  uint8_t writeHead = datapoints_writeHead[keyIndex];
  datapoints[keyIndex][writeHead] = {now, value};
  datapoints_writeHead[keyIndex] = (datapoints_writeHead[keyIndex] + 1) % WEBLOGGER_BUFFER_SIZE;

  xSemaphoreGive(datapoints_writeLock);
}

void WebLogger::log(WebLogLevel level, char* format, ...) {
  if (this->ws == NULL) {
    return;
  }

  char buffer[1024];
  va_list args;
  va_start (args, format);
  vsnprintf (buffer, 1024, format, args);
  va_end(args);

  StaticJsonDocument<1024> doc;
  doc["type"] = "log";
  doc["level"] = WebLogLevel_KEYS[static_cast<uint8_t>(level)];
  doc["msg"] = buffer;
  
  size_t len = measureJson(doc);
  AsyncWebSocketMessageBuffer *jsBuffer = ws->makeBuffer(len);
  serializeJson(doc, (char *)jsBuffer->get(), len + 1);
  ws->textAll(jsBuffer);
}

void WebLogger::sendData() {
  if (this->ws == NULL) {
    return;
  }

  if (xSemaphoreTake(this->datapoints_readLock, portMAX_DELAY) != pdTRUE) {
    // TODO - Log error
    return;
  }

  StaticJsonDocument<4064> doc;
  doc["type"] = "points";

  for (uint8_t i = 0; i < DataParameter_NUM_OF_ITEMS; i++) {
    uint8_t writeHead = datapoints_writeHead[i];
    uint8_t readHead = datapoints_readHead[i];
    uint8_t itemsToSend;

    if (writeHead > readHead) {
      // r x x x x w = 5 item (w = 5, r = 0, t = 6, w - r)
      // x r x x w x = 3 items (w = 4, r = 1, t = 6, w - r)
      itemsToSend = writeHead - readHead;
    } else if (readHead > writeHead) {
      // w x x x x r = 1 item (w = 0, r = 5, t = 6, (t - r) + w)
      // x w x x r x = 3 items (w = 1, r = 4, t = 6, (t - r) + w)
      itemsToSend = (WEBLOGGER_BUFFER_SIZE - readHead) + writeHead;
    } else { // readHead == writeHead
      continue;
    }

    const char* key = DataParameter_KEYS[i];
    JsonArray dataPoints = doc.createNestedArray(key);

    uint8_t head = readHead;
    for (uint8_t x = 0; x < itemsToSend; x++) {
      JsonArray point = dataPoints.createNestedArray();
      dataPoints.add(datapoints[i][head].time / 1000000.0);
      dataPoints.add(datapoints[i][head].value);
      head = (head + 1) % WEBLOGGER_BUFFER_SIZE;
    }

    datapoints_readHead[i] = writeHead;
  }

  size_t len = measureJson(doc);
  AsyncWebSocketMessageBuffer *buffer = ws->makeBuffer(len);
  serializeJson(doc, (char *)buffer->get(), len + 1);
  ws->textAll(buffer);

  xSemaphoreGive(datapoints_writeLock);
}