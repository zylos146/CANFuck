#include "motor/virtual.hpp"

void VirtualMotor::unsafeGoToPos(float position, float speed, float acceleration) {
  if (!this->isInState(MotorState::ACTIVE)) {
    ESP_LOGE("VirtualMotor", "Attempted to issue Motion CMD while in incorrect state '%s'!", this->getStateString());
    return;
  }

  
}

void VirtualMotor::stopMotion() {
  
}

void VirtualMotor::task_motion() {

}

static void virtualmot_run_task(void *pvParameter) {
  VirtualMotor* motor = (VirtualMotor*) pvParameter;

  // Wait for motor enable
  bool motorDisabled = true;
  while (motorDisabled) {
    motorDisabled = motor->hasStatusFlag(MOTOR_FLAG_ENABLED);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  
  //xTaskCreate(&linmot_heartbeat_task, "linmot_heartbeat_task", 4096, motor, 5, NULL);

  while (true) {
    motor->task_motion();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

VirtualMotor::VirtualMotor() {
  xTaskCreatePinnedToCore(&virtualmot_run_task, "virtualmot_run_task", 4096, this, 5, NULL, 1);
}