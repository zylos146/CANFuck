#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "linmot.h"
#include "CO_main.h"
#include "CO_linmot.h"

#include "esp_log.h"

#include "config.h"

uint16_t position = 0;
static void linmot_run_task(void *pvParameter) {
  while (true) {
    if (!comm_canopen_is_ready()) {

      vTaskDelay(250 / portTICK_PERIOD_MS);
      continue;
    }

    uint16_t statusWord;
    uint16_t runWord;
    uint16_t errorWord;
    uint16_t warnWord;

    OD_get_u16(OD_ENTRY_H2110_linMotStatus, 0x01, &statusWord, false);
    OD_get_u16(OD_ENTRY_H2110_linMotStatus, 0x02, &runWord, false);
    OD_get_u16(OD_ENTRY_H2110_linMotStatus, 0x03, &errorWord, false);
    OD_get_u16(OD_ENTRY_H2110_linMotStatus, 0x04, &warnWord, false);

    bool isEnabled = (statusWord & LINMOT_STATUS_OPERATION_ENABLED) > 0;
    bool isActive = (statusWord & LINMOT_STATUS_SWITCH_ON) > 0;
    bool isHomed = (statusWord & LINMOT_STATUS_HOMED) > 0;
    bool isMoving = (statusWord & LINMOT_STATUS_MOTION_ACTIVE) > 0;
    bool isError = (statusWord & LINMOT_STATUS_ERROR) > 0;

    if (isError && (errorWord == LINMOT_ERROR_CANBUS_GUARD_TIMEOUT)) {
      ESP_LOGI("task.main", "LinMot is in CanBus Guard Timeout state. Acknowledging!\n");

      linmot_control_add_flag(LINMOT_CONTROL_ERROR_ACKNOWLEDGE);
      vTaskDelay(50 / portTICK_PERIOD_MS);

      linmot_control_remove_flag(LINMOT_CONTROL_ERROR_ACKNOWLEDGE);
      vTaskDelay(50 / portTICK_PERIOD_MS);
      continue;
    }

    if (!isEnabled) {
      ESP_LOGI("task.main", "Switching on LinMot!\n");

      linmot_control_remove_flag(LINMOT_CONTROL_SWITCH_ON);
      vTaskDelay(50 / portTICK_PERIOD_MS);

      linmot_control_add_flag(LINMOT_CONTROL_SWITCH_ON);
      vTaskDelay(250 / portTICK_PERIOD_MS);
      continue;
    }

    if (isEnabled && !isHomed && !isMoving) {
      ESP_LOGI("task.main", "Homing LinMot!\n");
      // TODO - Should shut off homing state after this
      // This is why Motion CMD error is encountered after rebooting drive!

      // TODO - This is a hack to fix motion cmd error
      linmot_control_add_flag(LINMOT_CONTROL_HOME);
      vTaskDelay(5000 / portTICK_PERIOD_MS);
      linmot_control_remove_flag(LINMOT_CONTROL_HOME);
      vTaskDelay(50 / portTICK_PERIOD_MS);

      continue;
    }

    if (isEnabled && isActive && isHomed && !isMoving) {
      //ESP_LOGI("task.main", "Sending LinMot Motion CMD!\n");
      
      if (position == 0x01F4) {
        position = 0x03E8;
      } else {
        position = 0x01F4;
      }

      linmot_send_cmd(
        LINMOT_CMD_VAI_16BIT_GO_TO_POS,
        position, 0x03e8 * 5, 0x00ff, 0x00ff
      );

      vTaskDelay(100 / portTICK_PERIOD_MS);
      continue;
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

static void linmot_heartbeat_task() {
  bool hasInitialized = false;

  while (true) {
    if (comm_canopen_is_ready()) {
      if (hasInitialized == false) {
        /*Set Operating Mode of Slaves to Operational*/
        ESP_LOGI("task.main", "Setting LinMot to Operational State!\n");
        CO_NMT_sendCommand(CO->NMT, CO_NMT_ENTER_OPERATIONAL, NODE_ID_LINMOT);
        hasInitialized = true;
      }

      // It's been 1000ms since we've last gotten a LinMot Status update
      if (linmotPDOCounter > 1000) {
        ESP_LOGE("task.main", "Error: Have not recieved LinMot RPDO in %d ms! Attempting to re-establish!\n", linmotPDOCounter);
        CO_NMT_sendCommand(CO->NMT, CO_NMT_ENTER_OPERATIONAL, NODE_ID_LINMOT);
      }
    }

    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}

uint16_t controlWord = 0x003E;
void linmot_control_add_flag(uint16_t flag) {
    controlWord |= flag;
    OD_set_u16(OD_ENTRY_H2111_linMotControlWord, 0x00, controlWord, false);
    OD_requestTPDO(OD_variable_linMotControlWord_flagsPDO, 0);
}

void linmot_control_remove_flag(uint16_t flag) {
    controlWord &= ~flag;
    OD_set_u16(OD_ENTRY_H2111_linMotControlWord, 0x00, controlWord, false);
    OD_requestTPDO(OD_variable_linMotControlWord_flagsPDO, 0);
}

volatile uint8_t cmdCount = 0;
void linmot_send_cmd(uint16_t cmd, uint16_t parameter_a, uint16_t parameter_b, uint16_t parameter_c, uint16_t parameter_d) {
  // VAI 16Bit Go To Pos (090xh)
  cmdCount = (cmdCount + 1) % 16;
  uint16_t cmdWord = (cmd & 0xFFF0) | (cmdCount & 0x0F);
  OD_set_u16(OD_ENTRY_H2112_linMotCMD_Header, 0x00, cmdWord, false);

  OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x01, (uint8_t)((parameter_a >> 0) & 0xFF), false);
  OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x02, (uint8_t)((parameter_a >> 8) & 0xFF), false);

  OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x03, (uint8_t)((parameter_b >> 0) & 0xFF), false);
  OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x04, (uint8_t)((parameter_b >> 8) & 0xFF), false);

  OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x05, (uint8_t)((parameter_c >> 0) & 0xFF), false);
  OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x06, (uint8_t)((parameter_c >> 8) & 0xFF), false);

  OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x07, (uint8_t)((parameter_d >> 0) & 0xFF), false);
  OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x08, (uint8_t)((parameter_d >> 8) & 0xFF), false);

  // Triggers both TDPO 1 & 2
  OD_requestTPDO(OD_variable_linMotCMDParameters_flagsPDO, 0x01);
  OD_requestTPDO(OD_variable_linMotCMDParameters_flagsPDO, 0x07);
}

void linmot_register_tasks() {
  xTaskCreate(&linmot_run_task, "linmot_run_task", 4096, NULL, 5, NULL);
  xTaskCreate(&linmot_heartbeat_task, "linmot_heartbeat_task", 4096, NULL, 5, NULL);
}