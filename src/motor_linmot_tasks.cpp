#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "CO_main.h"

#include "esp_log.h"

#include "config.h"

#include "motor_linmot.hpp"

void LinmotMotor::task_motion() {
  OD_get_u16(this->CO_statusWord_entry, 0x01, &this->CO_statusWord, false);
  OD_get_u16(this->CO_statusWord_entry, 0x02, &this->CO_runWord, false);
  OD_get_u16(this->CO_statusWord_entry, 0x03, &this->CO_errorWord, false);
  OD_get_u16(this->CO_statusWord_entry, 0x04, &this->CO_warnWord, false);

  uint16_t statusWord = this->CO_statusWord;
  uint16_t errorWord = this->CO_errorWord;

  bool isEnabled = (statusWord & LINMOT_STATUS_OPERATION_ENABLED) > 0;
  // bool isActive = (statusWord & LINMOT_STATUS_SWITCH_ON) > 0;
  bool isHomed = (statusWord & LINMOT_STATUS_HOMED) > 0;
  bool isMoving = (statusWord & LINMOT_STATUS_MOTION_ACTIVE) > 0;
  bool isError = (statusWord & LINMOT_STATUS_ERROR) > 0;

  bool isCanTimeout = (errorWord == LINMOT_ERROR_CANBUS_GUARD_TIMEOUT);
  bool isMotionWrong = (errorWord == LINMOT_ERROR_MOTION_CMD_WRONG_STATE);
  if (isError && (isCanTimeout || isMotionWrong)) {
    ESP_LOGI("task.main", "LinMot is in a known error state. Acknowledging! Timeout %d - Motion %d\n", isCanTimeout, isMotionWrong);

    this->CO_control_addFlag(LINMOT_CONTROL_ERROR_ACKNOWLEDGE);
    vTaskDelay(50 / portTICK_PERIOD_MS);

    this->CO_control_removeFlag(LINMOT_CONTROL_ERROR_ACKNOWLEDGE);
    vTaskDelay(50 / portTICK_PERIOD_MS);

    return;
  }

  if (!isEnabled) {
    ESP_LOGI("task.main", "Switching on LinMot!\n");

    this->CO_control_removeFlag(LINMOT_CONTROL_SWITCH_ON);
    vTaskDelay(50 / portTICK_PERIOD_MS);

    this->CO_control_addFlag(LINMOT_CONTROL_SWITCH_ON);
    vTaskDelay(250 / portTICK_PERIOD_MS);

    // TODO - Move this to RPDO handler
    this->state = MotorState::ACTIVE;

    return;
  }

  if (isEnabled && !isHomed && !isMoving) {
    ESP_LOGI("task.main", "Homing LinMot!\n");
    // TODO - Should shut off homing state after this
    // This is why Motion CMD error is encountered after rebooting drive!

    // TODO - This is a hack to fix motion cmd error
    this->state = MotorState::HOMING;
    this->CO_control_addFlag(LINMOT_CONTROL_HOME);

    vTaskDelay(5000 / portTICK_PERIOD_MS);
    this->CO_control_removeFlag(LINMOT_CONTROL_HOME);
    vTaskDelay(50 / portTICK_PERIOD_MS);

    this->state = MotorState::ACTIVE;

    return;
  }
}

void LinmotMotor::task_heartbeat() {
  if (this->hasInitialized == false) {
    /*Set Operating Mode of Slaves to Operational*/
    ESP_LOGI("task.main", "Setting LinMot to Operational State!\n");
    CO_NMT_sendCommand(CO->NMT, CO_NMT_ENTER_OPERATIONAL, this->CO_nodeId);
    this->hasInitialized = true;
  }

  // It's been 1000ms since we've last gotten a LinMot Status update
  time_t now;
  time(&now);

  double diff = difftime(now, this->lastRPDOUpdate);
  if (diff > 1.0) {
    ESP_LOGE("task.main", "Error: Have not recieved LinMot RPDO in %d ms! Attempting to re-establish!\n", (int)(diff * 1000));
    CO_NMT_sendCommand(CO->NMT, CO_NMT_ENTER_OPERATIONAL, this->CO_nodeId);
  }
}

// Task Wrappers
static void linmot_run_task(void *pvParameter) {
  LinmotMotor* motor = (LinmotMotor*) pvParameter;

  while (true) {
    if (!comm_canopen_is_ready()) {
      vTaskDelay(250 / portTICK_PERIOD_MS);
      continue;
    }

    motor->task_motion();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void linmot_heartbeat_task(void * pvParameter) {
  LinmotMotor* motor = (LinmotMotor*) pvParameter;

  while (true) {
    if (comm_canopen_is_ready()) {
      motor->task_heartbeat();
    }

    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}

void LinmotMotor::registerTasks() {
  xTaskCreate(&linmot_run_task, "linmot_run_task", 4096, this, 5, NULL);
  xTaskCreate(&linmot_heartbeat_task, "linmot_heartbeat_task", 4096, this, 5, NULL);
}
