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
volatile uint32_t coInterruptCounter = 0U; /* variable increments each millisecond */
static void app_can_timer_task(void *arg);
esp_timer_create_args_t coMainTaskArgs;
esp_timer_handle_t periodicTimer;

CO_t *CO = NULL; /* CANopen object */
uint32_t heapMemoryUsed;
CO_config_t *config_ptr = NULL;

uint8_t LED_red, LED_green;

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

  /*Set Operating Mode of Slaves to Operational*/
  CO_NMT_sendCommand(CO->NMT, CO_NMT_ENTER_OPERATIONAL, NODE_ID_LINMOT);

  ESP_LOGI("init.comm", "done");
}

void app_loop() {

}

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
    }

    ESP_LOGI("task.main", "Communication reset was requested by CANOpen");
  }

  ESP_LOGI("task.main", "Application reset was requested by CANOpen");
  esp_restart();
}

static void app_can_timer_task(void *arg) {
  coInterruptCounter++;

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
}