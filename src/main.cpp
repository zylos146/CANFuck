#include "esp_log.h"
#include "Wire.h"
#include <Preferences.h>

#include "blynk.hpp"
#include "StrokeEngine.h"

#include "config.h"
#include "controller/canfuck.hpp"

#if MOTOR_USED==MOTOR_LINMOT
#include "motor/linmot_target.hpp"
#endif

#if MOTOR_USED==MOTOR_STEPPER
#include "motor/stepper_target.hpp"
#endif

static WiFiClient _blynkWifiClient;
static BlynkEsp32Client _blynkTransport(_blynkWifiClient);
BlynkWifi Blynk(_blynkTransport);

MotorInterface* motor;
StrokeEngine* engine;
CANFuckController* controller; // TODO - Abstract interface with controller away? Use similar system to Object Dictionary with CANOpen?

float position = 0;
void app_motion(void *pvParameter) {
  while (true) {
    // TODO - Doesn't restart after restarting the drive due to removal of slider
    if (motor->isInState(MotorState::ACTIVE) && motor->hasStatusFlag(MOTOR_FLAG_HOMED) && !engine->isActive()) {
      ESP_LOGE("main", "Motor ready and homed. Attempting to start Stroke Engine");
      engine->startPattern();
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void loop() {
  Blynk.run();
}

void setup() {
  Serial.begin(UART_SPEED);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  ESP_LOGI("main", "Starting Blynk");
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  if (CONTROLLER_USED) {
    ESP_LOGI("main", "Initializing Hardware Controller");
    controller = new CANFuckController();
    if (controller->start() == false) {
      ESP_LOGE("main", "Unable to initialize Hardware Controller");
      esp_restart();
    }
  } else {
    ESP_LOGI("main", "No Hardware Controller, only Blynk Controls will be available!");
  }

  ESP_LOGI("main", "Configuring Motor");
  motor = setupTargetMotor();

  ESP_LOGI("main", "Configuring Stroke Engine");
  engine = new StrokeEngine();
  engine->attachMotor(motor);
  engine->setParameter(StrokeParameter::PATTERN, 0);
  engine->setParameter(StrokeParameter::RATE, 50);
  engine->setParameter(StrokeParameter::DEPTH, 100);
  engine->setParameter(StrokeParameter::STROKE, 50);
  engine->setParameter(StrokeParameter::SENSATION, 0);

  if (controller != NULL) {
    controller->attachEngine(engine);
  }
  
  // TODO - Move controller tasks into init/attach
  if (controller != NULL) {
    controller->registerTasks();
  }

  ESP_LOGI("main", "Homing Motor");
  motor->enable();
  motor->goToHome();
  
  ESP_LOGI("main", "Starting Motion Task!");
  xTaskCreate(&app_motion, "app_motion", 4096, NULL, 5, NULL);
}

BLYNK_WRITE(BLYNK_SPEED) {
  engine->setParameter(StrokeParameter::RATE, param.asFloat());
}

BLYNK_WRITE(BLYNK_DEPTH) {
  engine->setParameter(StrokeParameter::DEPTH, param.asFloat());
}

BLYNK_WRITE(BLYNK_STROKE) {
  engine->setParameter(StrokeParameter::STROKE, param.asFloat());
}

BLYNK_WRITE(BLYNK_SENSATION) {
  engine->setParameter(StrokeParameter::SENSATION, param.asFloat());
}
