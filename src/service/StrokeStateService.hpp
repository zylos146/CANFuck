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
  float envelopeStart_mm = 0;
  float envelopeEnd_mm = 0;
  float strokes_per_min = 0;
  float sensation = 0;

  uint16_t pattern = 0;

  boolean in_estop = false;

  // TODO - Ensure a heartbeat is sent over Stroke Websocket. E-Stop machine if
  // 1. socket is closed
  // 2. heartbeat messages get no response within xxx ms
  // 3. shouldEstop is set to true

  static void read(StrokeState &settings, JsonObject &root)
  {
    root["envelopeStart_mm"] = settings.envelopeStart_mm;
    root["envelopeEnd_mm"] = settings.envelopeEnd_mm;
    root["strokes_per_min"] = settings.strokes_per_min;
    root["sensation"] = settings.sensation;
    root["pattern"] = settings.pattern;
    root["in_estop"] = settings.in_estop;
  }

  static StateUpdateResult update(JsonObject &root, StrokeState &state)
  {
    // TODO - Validations and limiting around this
    state.envelopeStart_mm = root["envelopeStart_mm"];
    state.envelopeEnd_mm = root["envelopeEnd_mm"];
    state.strokes_per_min = root["strokes_per_min"];
    state.sensation = root["sensation"];
    state.pattern = root["pattern"];
    state.in_estop = root["in_estop"];
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
    addUpdateHandler([&](const String &originId)
                     { onConfigUpdated(); },
                     false);
  }

  void begin(MachineStateService* machineState) {
    this->machineState = machineState;
    engine.attachMotor(&machineState->motor);
  }

  void onConfigUpdated() {
    read(([&](StrokeState state) { 
      if (state.in_estop) {
        // TODO - ESTOP machineState->motor.disable();
      }

      float depth = state.envelopeEnd_mm;
      float stroke = state.envelopeEnd_mm - state.envelopeStart_mm;

      engine.setParameter(StrokeParameter::PATTERN, state.pattern, false);
      engine.setParameter(StrokeParameter::RATE, state.strokes_per_min, false);
      engine.setParameter(StrokeParameter::DEPTH, depth, false);
      engine.setParameter(StrokeParameter::STROKE, stroke, false);
      engine.setParameter(StrokeParameter::SENSATION, state.sensation, true);
    }));
  }

private:
  HttpEndpoint<StrokeState> _httpEndpoint;
  WebSocketTxRx<StrokeState> _webSocket;

  StrokeEngine engine;
  MachineStateService* machineState;
};