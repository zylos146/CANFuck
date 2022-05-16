#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "CANopen.h"
#include "301/CO_config.h"
#include "OD.h"

#include "driver/gpio.h"
#include "driver/twai.h"
#include "esp_log.h"

#include "config.h"


// Periodic CAN Timer Task
volatile uint32_t coInterruptCounter = 0U;
static void app_can_timer_task(void *arg);
esp_timer_create_args_t coMainTaskArgs;
esp_timer_handle_t periodicTimer;

CO_t *CO = NULL; /* CANopen object */
uint32_t heapMemoryUsed;
CO_config_t *config_ptr = NULL;

uint8_t LED_red, LED_green;

OD_extension_t OD_variable_linMotCMDHeader_extension = {
    .object = NULL,
    .read = OD_readOriginal,
    .write = OD_writeOriginal
};
uint8_t *OD_variable_linMotCMDHeader_flagsPDO = NULL;


volatile uint32_t linmotPDOCounter = 0U;
ODR_t track_status_update(
  OD_stream_t *stream, const void *buf,
  OD_size_t count, OD_size_t *countWritten
) {
  linmotPDOCounter = 0;
  return OD_writeOriginal(stream, buf, count, countWritten);
}

OD_extension_t OD_variable_linMotStatus_extension = {
    .object = NULL,
    .read = OD_readOriginal,
    .write = track_status_update
};

void app_init_communication() {
  ESP_LOGI("init.comm", "Resetting communication...");
  CO_ReturnError_t err;
  uint32_t errInfo = 0;

  CO->CANmodule->CANnormal = false;
  // CO_CANsetConfigurationMode((void *)&CANptr); - Not needed on ESP32 - We handle this in CO_CANinit
  // TODO - ESP_ERR_INVALID_STATE - CO_CANmodule_disable(CO->CANmodule);

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

  // Initialize LinMot CMD Header as TPDO Mapped Object
  //  - Must be before CO_CANopenInitPDO
  //  - LinMot Parameters don't need to be mapped for OD_requestTPDO to work
  //  - Allows using OD_requestTPDO for triggering
  OD_extension_init(
    OD_ENTRY_H2112_linMotCMD_Header,
    &OD_variable_linMotCMDHeader_extension
  );
  OD_variable_linMotCMDHeader_flagsPDO = OD_getFlagsPDO(OD_ENTRY_H2112_linMotCMD_Header);

  // Initialize LinMot Status Words as RPDO Mapped Object
  //  - Used as a pesudo-heartbeat to detect alive-ness of LinMot
  OD_extension_init(
    OD_ENTRY_H2110_linMotStatus,
    &OD_variable_linMotStatus_extension
  );

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

  /* Configure Timer interrupt function for execution every CO_MAIN_TASK_INTERVAL */
  coMainTaskArgs.callback = &app_can_timer_task;
  coMainTaskArgs.name = "app_can_timer_task";
  ESP_ERROR_CHECK(esp_timer_create(&coMainTaskArgs, &periodicTimer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodicTimer, CO_MAIN_TASK_INTERVAL));

  /* start CAN */
  CO_CANsetNormalMode(CO->CANmodule);

  ESP_LOGI("init.comm", "done");
}

void app_loop() {

}

bool hasInitMot = false;
void app_can_main_task(void *pvParameter) {
  CO = CO_new(config_ptr, &heapMemoryUsed);
  if (CO == NULL) {
    ESP_LOGE("task.main", "Error: Can't allocate memory\n");
    return;
  } else {
    ESP_LOGI("task.main", "Allocated %u bytes for CANopen objects\n", heapMemoryUsed);
  }

  CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
  vTaskDelay(BOOT_WAIT / portTICK_PERIOD_MS);
  ESP_LOGI("task.main", "Started");

  while (reset != CO_RESET_APP) {
    uint32_t coInterruptCounterPrevious = coInterruptCounter;
    app_init_communication();
    reset = CO_RESET_NOT;

    while (reset == CO_RESET_NOT) {
      uint32_t coInterruptCounterCopy;
      uint32_t coInterruptCounterDiff;
      coInterruptCounterCopy = coInterruptCounter;
      coInterruptCounterDiff = coInterruptCounterCopy - coInterruptCounterPrevious;
      coInterruptCounterPrevious = coInterruptCounterCopy;

      //ESP_LOGI("task.main", "Processing CANOpen... %u diff cycles", coInterruptCounterDiff);
      reset = CO_process(CO, false, coInterruptCounterDiff * 1000, NULL);
      app_loop();

      vTaskDelay(MAIN_WAIT / portTICK_PERIOD_MS);

      if (hasInitMot == false) {
        /*Set Operating Mode of Slaves to Operational*/
        CO_NMT_sendCommand(CO->NMT, CO_NMT_ENTER_OPERATIONAL, NODE_ID_LINMOT);
        hasInitMot = true;
      }

      // It's been 1000ms since we've last gotten a LinMot Status update
      if (linmotPDOCounter > 1000) {
        ESP_LOGE("task.main", "Error: Have not recieved LinMot RPDO in %d ms! Attempting to re-establish!\n", linmotPDOCounter);
        CO_NMT_sendCommand(CO->NMT, CO_NMT_ENTER_OPERATIONAL, NODE_ID_LINMOT);
      }
    }

    ESP_LOGI("task.main", "Communication reset was requested by CANOpen");
  }

  ESP_LOGI("task.main", "Application reset was requested by CANOpen");
  esp_restart();
}

// TODO - Somehow integrate into main app task? What happens on app/comm reset?
// TODO - Ensure 16bit mapping is correct in Linmot
uint16_t position = 0;
static void app_linmot_task(void *pvParameter) {
  while (true) {
    // VAI 16Bit Go To Pos (090xh)
    OD_set_u16(OD_ENTRY_H2112_linMotCMD_Header, 0x01, 0x0901, false);

    // Target Position : 50mm
    if (position == 0x01F4) {
      position = 0x03E8;
    } else {
      position = 0x01F4;
    }
    OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x01, (uint8_t)((position && 0xff00) >> 2), false);
    OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x02, (uint8_t)((position && 0x00ff) >> 0), false);

    // Maximal Velocity : 1m/s
    OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x03, 0x03, false);
    OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x04, 0xE8, false);

    // Acceleration : 10m/s2 
    OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x05, 0x00, false);
    OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x06, 0x64, false);

    // De-acceleration : 10m/s2 
    OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x07, 0x00, false);
    OD_set_u8(OD_ENTRY_H2113_linMotCMD_Parameters, 0x08, 0x64, false);

    // Triggers both TDPO 1 & 2
    OD_requestTPDO(OD_variable_linMotCMDHeader_flagsPDO, 1);
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}

static void app_can_timer_task(void *arg) {
  coInterruptCounter++;
  linmotPDOCounter++;

  CO_LOCK_OD(CO->CANmodule);

  if (!CO->nodeIdUnconfigured && CO->CANmodule->CANnormal) {
    bool_t syncWas = false;

#if (CO_CONFIG_SYNC) & CO_CONFIG_SYNC_ENABLE
    syncWas = CO_process_SYNC(CO, CO_MAIN_TASK_INTERVAL, NULL);
#endif

#if (CO_CONFIG_PDO) & CO_CONFIG_RPDO_ENABLE
    CO_process_RPDO(CO, syncWas, CO_MAIN_TASK_INTERVAL, NULL);
#endif

#if (CO_CONFIG_PDO) & CO_CONFIG_TPDO_ENABLE
    CO_process_TPDO(CO, syncWas, CO_MAIN_TASK_INTERVAL, NULL);
#endif
  }

  CO_UNLOCK_OD(CO->CANmodule);
}

void app_main()
{
  ESP_LOGI("boot", "Attempting to start Main Task...");
  xTaskCreate(&app_can_main_task, "app_can_main_task", 4096, NULL, 5, NULL);
  xTaskCreate(&app_linmot_task, "app_linmot_task", 4096, NULL, 5, NULL);
}