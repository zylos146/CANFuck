#include "CO_main.h"
#include "motor_linmot.hpp"
#include "esp_log.h"

#include "blynk.hpp"

static WiFiClient _blynkWifiClient;
static BlynkEsp32Client _blynkTransport(_blynkWifiClient);
BlynkWifi Blynk(_blynkTransport);

char ssid[] = "DESKTOP-7FGKC64 1018";
char pass[] = "taobao123";

LinmotMotor* motor;
float position = 0;
void app_motion(void *pvParameter) {
  while (true) {
    if (motor->isInState(MotorState::ACTIVE) && motor->hasStatusFlag(MOTOR_FLAG_HOMED)) {
      
      if (position == 500) {
        position = 1000;
      } else {
        position = 500;
      }

      motor->goToPos(position, 500, 2500);
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void loop() {
  Blynk.run();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

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
  motor->CO_setStatus(OD_ENTRY_H2110_linMotStatus);
  motor->CO_setControl(OD_ENTRY_H2111_linMotControlWord);
  motor->CO_setCmdHeader(OD_ENTRY_H2112_linMotCMD_Header);
  motor->CO_setCmdParameters(OD_ENTRY_H2113_linMotCMD_Parameters);

  // CO Startup must occur after motor CO configuration due to OD extension initialization
  CO_register_tasks();
  motor->registerTasks();

  motor->enable();

  motor->goToHome();

  xTaskCreate(&app_motion, "app_motion", 4096, NULL, 5, NULL);
  //xTaskCreate(&app_blynk, "app_blynk", 4096, NULL, 5, NULL);
}