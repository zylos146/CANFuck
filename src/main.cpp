#include "CO_main.h"
#include "motor_linmot.hpp"
#include "esp_log.h"

#include "blynk.hpp"
#include "StrokeEngine.h"

static WiFiClient _blynkWifiClient;
static BlynkEsp32Client _blynkTransport(_blynkWifiClient);
BlynkWifi Blynk(_blynkTransport);

char ssid[] = "DESKTOP-7FGKC64 1018";
char pass[] = "taobao123";

LinmotMotor* motor;
StrokeEngine* engine = NULL;

float position = 0;
void app_motion(void *pvParameter) {
  while (true) {
    if (motor->isInState(MotorState::ACTIVE) && motor->hasStatusFlag(MOTOR_FLAG_HOMED)) {
      ESP_LOGE("main", "Motor ready and homed. Attempting to start Stroke Engine");
      engine->ready();
      engine->startPattern();
      vTaskSuspend(NULL);
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void loop() {
  Blynk.run();
}

void setup() {
  Serial.begin(115200);

  ESP_LOGI("main", "Starting Blynk");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);


  ESP_LOGI("main", "Configuring Motor");
  motor = new LinmotMotor();

  motionBounds bounds = {
    .start = 110, // mm
    .end = 0, // mm
    .keepout = 5 // mm
  };
  motor->setMaxSpeed(5000); // 5 m/s
  motor->setMaxAcceleration(25000); // 25 m/s^2
  motor->setBounds(bounds);

  motor->CO_setNodeId(NODE_ID_LINMOT);
  motor->CO_setStatus(OD_ENTRY_H2110_linMotStatusUInt16);
  motor->CO_setMonitor(OD_ENTRY_H2114_linMotStatusSInt16);
  motor->CO_setControl(OD_ENTRY_H2111_linMotControlWord);
  motor->CO_setCmdHeader(OD_ENTRY_H2112_linMotCMD_Header);
  motor->CO_setCmdParameters(OD_ENTRY_H2113_linMotCMD_Parameters);

  ESP_LOGI("main", "Configuring Stroke Engine");
  engine = new StrokeEngine();
  engine->attachMotor(motor);
  engine->setParameter(StrokeParameter::PATTERN, 0);
  engine->setParameter(StrokeParameter::RATE, 240);
  engine->setParameter(StrokeParameter::DEPTH, 100);
  engine->setParameter(StrokeParameter::STROKE, 50);
  engine->setParameter(StrokeParameter::SENSATION, 0);

  ESP_LOGI("main", "Registering Motor Tasks");
  // CO Startup must occur after motor CO configuration due to OD extension initialization
  CO_register_tasks();
  motor->registerTasks();

  ESP_LOGI("main", "Homing Motor");
  motor->enable();

  motor->goToHome();
  
  ESP_LOGI("main", "Starting Motion Task!");
  xTaskCreate(&app_motion, "app_motion", 4096, NULL, 5, NULL);
  //xTaskCreate(&app_blynk, "app_blynk", 4096, NULL, 5, NULL);
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
