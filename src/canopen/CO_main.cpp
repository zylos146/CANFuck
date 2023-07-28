
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "CO_main.h"
#include "motor.hpp"

#include "esp_log.h"


#define OD_LINMOT_CONTROL_WORD 0x21110010
#define OD_LINMOT_CMD_HEADER   0x21120010
#define OD_LINMOT_CMD_PARAM_0  0x21130108
#define OD_LINMOT_CMD_PARAM_1  0x21130208
#define OD_LINMOT_CMD_PARAM_2  0x21130308
#define OD_LINMOT_CMD_PARAM_3  0x21130408
#define OD_LINMOT_CMD_PARAM_4  0x21130508
#define OD_LINMOT_CMD_PARAM_5  0x21130608
#define OD_LINMOT_CMD_PARAM_6  0x21130708
#define OD_LINMOT_CMD_PARAM_7  0x21130808

CO_t *CO = NULL;
CO_NMT_reset_cmd_t reset = CO_RESET_COMM;

bool comm_canopen_is_ready() {
  return reset == CO_RESET_NOT;
  // TODO - Check this works right
  //return CO->NMT->operatingState == CO_NMT_OPERATIONAL;
}

uint32_t heapMemoryUsed;
CO_config_t *config_ptr = NULL;

uint8_t LED_red, LED_green;

int64_t coLastProcessMicro = 0;

void CO_linmot_b1100_TPDO_init() {
  // Disable TPDO via Number of mapped objects = 0
  OD_set_u8(OD_ENTRY_H1A00_TPDOMappingParameter, 0x00, 0, false);
  OD_set_u8(OD_ENTRY_H1A01_TPDOMappingParameter, 0x00, 0, false);
  OD_set_u8(OD_ENTRY_H1A02_TPDOMappingParameter, 0x00, 0, false);

  OD_set_u32(OD_ENTRY_H1A00_TPDOMappingParameter, 0x01, OD_LINMOT_CONTROL_WORD, false);

  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x01, OD_LINMOT_CMD_HEADER, false);
  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x02, OD_LINMOT_CMD_PARAM_0, false);
  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x03, OD_LINMOT_CMD_PARAM_1, false);
  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x04, OD_LINMOT_CMD_PARAM_2, false);
  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x05, OD_LINMOT_CMD_PARAM_3, false);
  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x06, OD_LINMOT_CMD_PARAM_4, false);
  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x07, OD_LINMOT_CMD_PARAM_5, false);
  
  OD_set_u32(OD_ENTRY_H1A02_TPDOMappingParameter, 0x01, OD_LINMOT_CMD_HEADER, false);
  OD_set_u32(OD_ENTRY_H1A02_TPDOMappingParameter, 0x02, OD_LINMOT_CMD_PARAM_6, false);
  OD_set_u32(OD_ENTRY_H1A02_TPDOMappingParameter, 0x03, OD_LINMOT_CMD_PARAM_7, false);
  
  OD_set_u8(OD_ENTRY_H1A00_TPDOMappingParameter, 0x00, 1, false);
  OD_set_u8(OD_ENTRY_H1A01_TPDOMappingParameter, 0x00, 7, false);
  OD_set_u8(OD_ENTRY_H1A02_TPDOMappingParameter, 0x00, 3, false);
}

void CO_linmot_a1100_TPDO_init() {
  // Disable TPDO via Number of mapped objects = 0
  OD_set_u8(OD_ENTRY_H1A00_TPDOMappingParameter, 0x00, 0, false);
  OD_set_u8(OD_ENTRY_H1A01_TPDOMappingParameter, 0x00, 0, false);
  OD_set_u8(OD_ENTRY_H1A02_TPDOMappingParameter, 0x00, 0, false);

  OD_set_u32(OD_ENTRY_H1A00_TPDOMappingParameter, 0x01, OD_LINMOT_CONTROL_WORD, false);
  OD_set_u32(OD_ENTRY_H1A00_TPDOMappingParameter, 0x02, OD_LINMOT_CMD_HEADER, false);
  OD_set_u32(OD_ENTRY_H1A00_TPDOMappingParameter, 0x03, OD_LINMOT_CMD_PARAM_0, false);
  OD_set_u32(OD_ENTRY_H1A00_TPDOMappingParameter, 0x04, OD_LINMOT_CMD_PARAM_1, false);
  OD_set_u32(OD_ENTRY_H1A00_TPDOMappingParameter, 0x05, OD_LINMOT_CMD_PARAM_2, false);
  OD_set_u32(OD_ENTRY_H1A00_TPDOMappingParameter, 0x06, OD_LINMOT_CMD_PARAM_3, false);

  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x01, OD_LINMOT_CMD_HEADER, false);
  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x02, OD_LINMOT_CMD_PARAM_4, false);
  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x03, OD_LINMOT_CMD_PARAM_5, false);
  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x04, OD_LINMOT_CMD_PARAM_6, false);
  OD_set_u32(OD_ENTRY_H1A01_TPDOMappingParameter, 0x05, OD_LINMOT_CMD_PARAM_7, false);
  
  OD_set_u8(OD_ENTRY_H1A00_TPDOMappingParameter, 0x00, 6, false);
  OD_set_u8(OD_ENTRY_H1A01_TPDOMappingParameter, 0x00, 5, false);
  OD_set_u8(OD_ENTRY_H1A02_TPDOMappingParameter, 0x00, 0, false);
}

void app_init_communication() {
  ESP_LOGI("init.comm", "Resetting communication...");
  CO_ReturnError_t err;
  uint32_t errInfo = 0;

  CO->CANmodule->CANnormal = false;
  // CO_CANsetConfigurationMode((void *)&CANptr); - Not needed on ESP32 - We handle this in CO_CANinit
  // TODO - ESP_ERR_INVALID_STATE - CO_CANmodule_disable(CO->CANmodule);

  if (CAN_OD_USED == CAN_A1100_OD) {
    CO_linmot_a1100_TPDO_init();
  } else {
    CO_linmot_b1100_TPDO_init();
  }

  /* Initialize CAN Device */
  err = CO_CANinit(CO, NULL, CAN_BITRATE /* bit rate */);
  if (err != CO_ERROR_NO)
  {
    ESP_LOGE("init.comm", "CO_init failed. Errorcode: %d", err);
    CO_errorReport(CO->em, CO_EM_MEMORY_ALLOCATION_ERROR, CO_EMC_SOFTWARE_INTERNAL, err);
    esp_restart();
  }

  /* Initialize CANOpen services as defined in CO_config.h */
  err = CO_CANopenInit(
    CO,                /* CANopen object */
    NULL,              /* alternate NMT */
    NULL,              /* alternate em */
    OD,                /* Object dictionary */
    OD_STATUS_BITS,    /* Optional OD_statusBits */
    NMT_CONTROL,       /* CO_NMT_control_t */
    FIRST_HB_TIME,     /* firstHBTime_ms */
    SDO_SRV_TIMEOUT_TIME, /* SDOserverTimeoutTime_ms */
    SDO_CLI_TIMEOUT_TIME, /* SDOclientTimeoutTime_ms */
    SDO_CLI_BLOCK,     /* SDOclientBlockTransfer */
    NODE_ID_SELF,
    &errInfo
  );

  if(err != CO_ERROR_NO && err != CO_ERROR_NODE_ID_UNCONFIGURED_LSS) {
    if (err == CO_ERROR_OD_PARAMETERS) {
      ESP_LOGE("init.comm", "Error: Object Dictionary entry 0x%X\n", errInfo);
    } else {
      ESP_LOGE("init.comm", "Error: CANopen initialization failed: %d\n", err);
    }
    return;
  }

  /* Initialize TPDO and RPDO if defined in CO_config.h */
  err = CO_CANopenInitPDO(CO, CO->em, OD, NODE_ID_SELF, &errInfo);
  if(err != CO_ERROR_NO) {
    if (err == CO_ERROR_OD_PARAMETERS) {
      ESP_LOGE("init.comm", "Error: Object Dictionary entry 0x%X\n", errInfo);
    } else {
      ESP_LOGE("init.comm", "Error: PDO initialization failed: %d\n", err);
    }
    return;
  }

  /* start CAN */
  CO_CANsetNormalMode(CO->CANmodule);

  ESP_LOGI("init.comm", "done");
}

void canopen_main_task(void *pvParameter) {
  CO = CO_new(config_ptr, &heapMemoryUsed);
  if (CO == NULL) {
    ESP_LOGE("task.main", "Error: Can't allocate memory\n");
    return;
  } else {
    ESP_LOGI("task.main", "Allocated %u bytes for CANopen objects\n", heapMemoryUsed);
  }

  vTaskDelay(CAN_BOOT_DELAY / portTICK_PERIOD_MS);
  ESP_LOGI("task.main", "Started");

  while (reset != CO_RESET_APP) {
    app_init_communication();
    reset = CO_RESET_NOT;

    while (reset == CO_RESET_NOT) {
      int64_t coLastProcessMicroDiff;
      int64_t coLastProcessMicroNow = esp_timer_get_time();
      coLastProcessMicroDiff = coLastProcessMicroNow - coLastProcessMicro;
      coLastProcessMicro = coLastProcessMicroNow;

      //ESP_LOGI("task.main", "Processing CANOpen... %u diff cycles", coInterruptCounterDiff);
      reset = CO_process(CO, false, coLastProcessMicroDiff, NULL);

      CO_LOCK_OD(CO->CANmodule);
      if (!CO->nodeIdUnconfigured && CO->CANmodule->CANnormal) {
        bool_t syncWas = false;

    #if (CO_CONFIG_SYNC) & CO_CONFIG_SYNC_ENABLE
        syncWas = CO_process_SYNC(CO, coLastProcessMicroDiff, NULL);
    #endif

    #if (CO_CONFIG_PDO) & CO_CONFIG_RPDO_ENABLE
        CO_process_RPDO(CO, syncWas, coLastProcessMicroDiff, NULL);
    #endif

    #if (CO_CONFIG_PDO) & CO_CONFIG_TPDO_ENABLE
        CO_process_TPDO(CO, syncWas, coLastProcessMicroDiff, NULL);
    #endif
      }

      CO_UNLOCK_OD(CO->CANmodule);

      vTaskDelay(CANOPEN_TASK_INTERVAL_MS / portTICK_PERIOD_MS);
    }

    ESP_LOGI("task.main", "Communication reset was requested by CANOpen");
  }

  ESP_LOGI("task.main", "Application reset was requested by CANOpen");
  esp_restart();
}

void CO_register_tasks() {
  xTaskCreatePinnedToCore(&canopen_main_task, "canopen_main_task", 4096 * 4, NULL, 2, NULL, 1);
}