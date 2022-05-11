#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "CANopen.h"
#include "CO_OD.h"
#include "CO_config.h"

#include "driver/gpio.h"
#include "driver/twai.h"
#include "esp_log.h"

#include "config.h"

// Periodic CAN Timer Task
volatile uint32_t coInterruptCounter = 0U; /* variable increments each millisecond */
static void app_can_timer_task(void *arg);
esp_timer_create_args_t coMainTaskArgs;
esp_timer_handle_t periodicTimer;

void app_init_communication() {
  ESP_LOGI("init.comm", "initializing...");
  CO_ReturnError_t err;

  /* initialize CANopen */
  err = CO_init(NULL, NODE_ID_SELF /* NodeID */, CAN_BITRATE /* bit rate */);
  if (err != CO_ERROR_NO)
  {
    ESP_LOGE("init.comm", "CO_init failed. Errorcode: %d", err);
    CO_errorReport(CO->em, CO_EM_MEMORY_ALLOCATION_ERROR, CO_EMC_SOFTWARE_INTERNAL, err);
    esp_restart();
  }

  /* Configure Timer interrupt function for execution every CO_MAIN_TASK_INTERVAL */
  coMainTaskArgs.callback = &app_can_timer_task;
  coMainTaskArgs.name = "app_can_timer_task";
  ESP_ERROR_CHECK(esp_timer_create(&coMainTaskArgs, &periodicTimer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodicTimer, CO_MAIN_TASK_INTERVAL));

  /* start CAN */
  CO_CANsetNormalMode(CO->CANmodule[0]);

  /*Set Operating Mode of Slaves to Operational*/
  CO_sendNMTcommand(CO, 0x01, NODE_ID_LINMOT);

  ESP_LOGI("init.comm", "done");
}

void app_loop() {

}

void app_can_main_task(void *pvParameter) {
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
      reset = CO_process(CO, coInterruptCounterDiff, NULL);
      app_loop();

      vTaskDelay(MAIN_WAIT / portTICK_PERIOD_MS);
    }

    ESP_LOGI("task.main", "Communication reset was requested by CANOpen");
  }

  ESP_LOGI("task.main", "Application reset was requested by CANOpen");
  esp_restart();
}

static void app_can_timer_task(void *arg)
{
  coInterruptCounter++;

  if (CO->CANmodule[0]->CANnormal)
  {
    bool_t syncWas;

    /* Process Sync */
    syncWas = CO_process_SYNC(CO, CO_MAIN_TASK_INTERVAL);

    /* Read inputs */
    CO_process_RPDO(CO, syncWas);

    /* Write outputs */
    CO_process_TPDO(CO, syncWas, CO_MAIN_TASK_INTERVAL);
  }
}

void app_main()
{
  ESP_LOGI("boot", "Attempting to start Main Task...");
  xTaskCreate(&app_can_main_task, "app_can_main_task", 4096, NULL, 5, NULL);
}