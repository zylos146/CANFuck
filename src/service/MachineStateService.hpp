#pragma once

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include "motor/linmot.hpp"
#include "CO_main.h"

#define MACHINE_CONFIG_ENDPOINT_PATH "/rest/machine/config"
#define MACHINE_HOME_ENDPOINT_PATH "/rest/machine/home"

// TODO - Disallow changes to Machine Config once a Stroke has started, or motor has energized?

class StrokeStateService;
class MachineState
{
public:
  float machineStart_mm;
  float machineEnd_mm;
  float machineKeepout_mm;
  float machineMaxSpeed_ms = 5000; // 5 m/s
  float machineMaxAccel_mss = 500; // 25 m/s^2
  // motorUsed
  // controllerUsed

  static void read(MachineState &settings, JsonObject &root)
  {
    root["machineStart_mm"] = settings.machineStart_mm;
    root["machineEnd_mm"] = settings.machineEnd_mm;
    root["machineKeepout_mm"] = settings.machineKeepout_mm;
    root["machineMaxSpeed_ms"] = settings.machineMaxSpeed_ms;
    root["machineMaxAccel_mss"] = settings.machineMaxAccel_mss;
  }

  static StateUpdateResult update(JsonObject &root, MachineState &state)
  {
    state.machineStart_mm = root["machineStart_mm"];
    state.machineEnd_mm = root["machineEnd_mm"];
    state.machineKeepout_mm = root["machineKeepout_mm"];
    state.machineMaxSpeed_ms = root["machineMaxSpeed_ms"];
    state.machineMaxAccel_mss = root["machineMaxAccel_mss"];

    return StateUpdateResult::CHANGED;
  }
};

class MachineStateService : public StatefulService<MachineState>
{
public:
  MachineStateService(
    AsyncWebServer *server,
    SecurityManager *securityManager,
    FS* fs
  ) : 
    _httpEndpoint(MachineState::read,
      MachineState::update,
      this,
      server,
      MACHINE_CONFIG_ENDPOINT_PATH,
      securityManager,
      AuthenticationPredicates::IS_AUTHENTICATED
    ),
    _fsPersistence(
      MachineState::read, 
      MachineState::update, 
      this, 
      fs, 
      "/config/machine.json"
    )
  {
    log_i("Creating CANfuck Machine Service");

    motor.CO_setNodeId(NODE_ID_LINMOT);
    motor.CO_setStatusUInt16(OD_ENTRY_H2110_linMotStatusUInt16);
    motor.CO_setMonitorSInt16(OD_ENTRY_H2114_linMotStatusSInt16);
    motor.CO_setMonitorSInt32(OD_ENTRY_H2115_linMotStatusSInt32);
    motor.CO_setControl(OD_ENTRY_H2111_linMotControlWord);
    motor.CO_setCmdHeader(OD_ENTRY_H2112_linMotCMD_Header);
    motor.CO_setCmdParameters(OD_ENTRY_H2113_linMotCMD_Parameters);

    server->on(
        MACHINE_HOME_ENDPOINT_PATH,
        HTTP_POST,
        securityManager->wrapRequest(
            std::bind(&MachineStateService::doHome, this, std::placeholders::_1),
            AuthenticationPredicates::IS_ADMIN));

    addUpdateHandler([&](const String &originId)
                     { onConfigUpdated(); },
                     false);

    log_i("Created CANfuck Machine Service");
  }

  void begin() {
    _fsPersistence.readFromFS();
    updateMotor();
  }

  void onConfigUpdated() {
    read(([&](MachineState state) { 
      updateMotor();
    }));
  }

  void updateMotor() {
    bounds = {
      .start = _state.machineStart_mm, // mm
      .end = _state.machineEnd_mm, // mm
      .keepout = _state.machineKeepout_mm // mm
    };
    
    motor.setMaxSpeed(_state.machineMaxSpeed_ms); 
    motor.setMaxAcceleration(_state.machineMaxAccel_mss); 
    motor.setMachineGeometry(bounds);
  }

  void doHome(AsyncWebServerRequest *request)
  {
    motor.startHoming();
    request->send(200);
  }

  friend class StrokeStateService;

private:
  HttpEndpoint<MachineState> _httpEndpoint;
  FSPersistence<MachineState> _fsPersistence;

  LinmotMotor motor;
  MachineGeometry bounds;
};