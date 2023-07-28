/*
 the assumptions are the same as for vprintf function. In IDF, vprintf is reentrant because the I/O streams have locks, 
 so only one task can vprintf to a given stream at any point in time. Also keep in mind the situation when your 
 custom vprintf implementation causes esp_log_write to be called. 

 For example, this may happen if SPIFFS driver emits some warning or error message. So you also need to consider 
 how to handle recursive entry into your custom vprintf function in a given thread.
*/

// https://github.com/MalteJ/embedded-esp32-component-udp_logging/blob/master/udp_logging.c

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include <functional>
#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>

#include "StrokeEngine.h"
#include "MachineStateService.hpp"

// GET /rest/machine/config
// POST /rest/machine/config

// POST /rest/machine/home
// POST /rest/machine/

// GET /rest/stroke
//
// /ws/stroke

#define LOGGING_ENDPOINT_PATH "/rest/logs"
#define LOGGING_SOCKET_PATH "/ws/logs"   

class LoggingState
{
public:

  static void read(LoggingState &settings, JsonObject &root)
  {
    //root["envelopeStart_mm"] = settings.envelopeStart_mm;
  }

  static StateUpdateResult update(JsonObject &root, LoggingState &state)
  {
    // TODO - Validations and limiting around this
    //state.envelopeStart_mm = root["envelopeStart_mm"];
    return StateUpdateResult::CHANGED;
  }
};

class LoggingStateService : public StatefulService<LoggingState>
{
public:
  // LoggingStateService(AsyncWebServer *server,
  //                   SecurityManager *securityManager);

  LoggingStateService(AsyncWebServer *server, SecurityManager *securityManager);

  void begin();

  void onWSEvent(
    AsyncWebSocket *server,
    AsyncWebSocketClient *client,
    AwsEventType type,
    void *arg,
    uint8_t *data,
    size_t len
  );

  void onConfigUpdated() {}

private:
  HttpEndpoint<LoggingState> _httpEndpoint;
  AsyncWebSocket _webSocket;
  AsyncWebSocketClient *client = NULL; // TODO - Only support one client right now

  static void taskSendLogs(void* parameter);

  void forbidden(AsyncWebServerRequest *request)
  {
      request->send(403);
  }
};
