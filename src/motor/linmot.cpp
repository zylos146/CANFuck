#include "motor/linmot.hpp"
#include "esp_log.h"

void LinmotMotor::unsafeGoToPos(MachinePosition position, float speed, float acceleration) {
  if (!this->isInState(MotorState::RUNNING)) {
    ESP_LOGE("LinmotMotor", "Attempted to issue Motion CMD while in incorrect state '%s'!", this->getStateString());
    return;
  }

  ESP_LOGD("motor", "Going to real position %05.1f mm @ %05.1f mm/s, %05.1f mm/s^2", position, speed, acceleration);
  this->CO_sendCmd(
    0x0900, 
    static_cast<uint16_t>(position * 10), 
    static_cast<uint16_t>(speed), 
    static_cast<uint16_t>(acceleration), 
    static_cast<uint16_t>(acceleration)
  );
}

bool LinmotMotor::isUnpowered() {
  uint8_t runState = (this->CO_runWord & 0xff00) >> 8;
  return runState == LINMOT_STATE_NOT_READY_TO_SWITCH_ON || runState == LINMOT_STATE_SWITCH_ON_DISABLED || runState == LINMOT_STATE_READY_TO_SWITCH_ON;
}

bool LinmotMotor::isStopped() {
  uint8_t runState = (this->CO_runWord & 0xff00) >> 8;
  return runState != LINMOT_STATE_OPERATIONAL;
}

bool LinmotMotor::isRunning() {
  uint8_t runState = (this->CO_runWord & 0xff00) >> 8;
  // TODO - This will be based on if it accept motion commands
  return runState == LINMOT_STATE_OPERATIONAL;
}

bool LinmotMotor::isHoming() {
  uint8_t runState = (this->CO_runWord & 0xff00) >> 8;
  return runState == LINMOT_STATE_HOMING;
}

bool LinmotMotor::hasFault() {
  uint8_t runState = (this->CO_runWord & 0xff00) >> 8;

  // If the motor isn't in a recognized and handled state, we will consider it having faulted
  if (!isUnpowered() || !isStopped() || !isRunning() || !isHoming()) {
    return false;
  }

  return runState == LINMOT_STATE_ERROR;
}