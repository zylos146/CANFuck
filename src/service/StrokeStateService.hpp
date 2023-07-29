#pragma once

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

#define STROKE_ENDPOINT_PATH "/rest/stroke"
#define STROKE_SOCKET_PATH "/ws/stroke"

class StrokeState
{
public:
  float envelope_start_mm = 0;
  float envelope_end_mm = 0;
  float strokes_per_min = 0;
  float sensation = 0;

  uint16_t pattern = 0;

  boolean in_estop = false;
  boolean running = false;

  // TODO - Ensure a heartbeat is sent over Stroke Websocket. E-Stop machine if
  // 1. socket is closed
  // 2. heartbeat messages get no response within xxx ms
  // 3. shouldEstop is set to true

  static void read(StrokeState &settings, JsonObject &root)
  {
    root["envelope_start_mm"] = settings.envelope_start_mm;
    root["envelope_end_mm"] = settings.envelope_end_mm;
    root["strokes_per_min"] = settings.strokes_per_min;
    root["sensation"] = settings.sensation;
    root["pattern"] = settings.pattern;
    root["in_estop"] = settings.in_estop;
    root["running"] = settings.running;
  }

  static StateUpdateResult update(JsonObject &root, StrokeState &state)
  {
    // TODO - Validations and limiting around this
    state.envelope_start_mm = root["envelope_start_mm"];
    state.envelope_end_mm = root["envelope_end_mm"];
    state.strokes_per_min = root["strokes_per_min"];
    state.sensation = root["sensation"];
    state.pattern = root["pattern"];
    state.in_estop = root["in_estop"];
    state.running = root["running"];
    return StateUpdateResult::CHANGED;
  }
};

class StrokeStateService : public StatefulService<StrokeState>
{
public:
  // StrokeStateService(AsyncWebServer *server,
  //                   SecurityManager *securityManager);

  StrokeStateService(AsyncWebServer *server, SecurityManager *securityManager) : 
    _httpEndpoint(
      StrokeState::read,
      StrokeState::update,
      this,
      server,
      STROKE_ENDPOINT_PATH,
      securityManager,
      AuthenticationPredicates::IS_AUTHENTICATED
    ),
    _webSocket(
      StrokeState::read,
      StrokeState::update,
      this,
      server,
      STROKE_SOCKET_PATH,
      securityManager,
      AuthenticationPredicates::IS_AUTHENTICATED
    )
  {
    log_i("Created CANfuck Stroke Service");
  }

  void begin(MachineStateService* machineState) {
    this->machineState = machineState;
    engine.attachMotor(&(machineState->motor));
    log_i("Attached motor to stroke engine"); // TODO - This shoudl be moved into stroke engine
    
    addUpdateHandler([&](const String &originId)
                     { onConfigUpdated(); },
                     false);

  }

  // TODO - https://github.com/me-no-dev/ESPAsyncWebServer/issues/1158#issuecomment-1557017468
  // Disconnect phone from ESP32 hotspot, gets up with overflow spam

  void onConfigUpdated() {
    read(([&](StrokeState state) { 
      // Set Parameters
      float depth = state.envelope_end_mm;
      float stroke = state.envelope_end_mm - state.envelope_start_mm;

      engine.setParameter(StrokeParameter::PATTERN, state.pattern, false);
      engine.setParameter(StrokeParameter::RATE, state.strokes_per_min, false);
      engine.setParameter(StrokeParameter::DEPTH, depth, false);
      engine.setParameter(StrokeParameter::STROKE, stroke, false);
      engine.setParameter(StrokeParameter::SENSATION, state.sensation, true);

      // Setup motor / engine
      if (state.in_estop == false && machineState->motor.isStopped()) {
        machineState->motor.powerUp();
      }

      if (state.in_estop == true && !machineState->motor.isStopped()) {
        engine.stopPattern();
        machineState->motor.powerDown();
      }

      if (state.running == false && machineState->motor.isRunning()) {
        engine.stopPattern();
        //machineState->motor.stopMotion();
      }

      if (state.running == true && !machineState->motor.isRunning()) {
        //machineState->motor.allowMotion();
        engine.startPattern();
      }
    }));
  }

private:
  HttpEndpoint<StrokeState> _httpEndpoint;
  WebSocketTxRx<StrokeState> _webSocket;

  StrokeEngine engine;
  MachineStateService* machineState;
};