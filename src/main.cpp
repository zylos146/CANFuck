#include "esp_log.h"
#include "Wire.h"
#include <Preferences.h>

#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

#include "blynk.hpp"
#include "StrokeEngine.h"

#include "config.h"
#include "controller/canfuck.hpp"

#include "CO_main.h"
#include "motor/linmot.hpp"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws

static WiFiClient _blynkWifiClient;
static BlynkEsp32Client _blynkTransport(_blynkWifiClient);
BlynkWifi Blynk(_blynkTransport);

// TODO - Allow ESP32 Perferences to choose motor implementation
LinmotMotor* motor;
StrokeEngine* engine;
CANFuckController* controller; // TODO - Abstract interface with controller away? Use similar system to Object Dictionary with CANOpen?

void onRequest(AsyncWebServerRequest *request){
  //Handle Unknown Request
  request->send(404);
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  //Handle body
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  //Handle upload
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  //Handle WebSocket event
}

void loop() {
  Blynk.run();
}

void setup() {
  Serial.begin(UART_SPEED);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  ESP_LOGI("main", "Mounting SPIFFS");
  if(!SPIFFS.begin(true)){
    ESP_LOGE("main", "An Error has occurred while mounting SPIFFS");
    return;
  }

  ESP_LOGI("main", "Starting Blynk");
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  
  ESP_LOGI("main", "Starting Web Server");
  server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  server.onNotFound([](AsyncWebServerRequest *request) {
      Serial.printf("Not found: %s!\r\n", request->url().c_str());
      request->send(404);
   });

  server.begin();

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
  motor = new LinmotMotor();

  MachineGeometry bounds = {
    .start = 110, // mm
    .end = -7, // mm
    .keepout = 10 // mm
  };
  motor->setMaxSpeed(5000); // 5 m/s
  motor->setMaxAcceleration(25000); // 25 m/s^2
  motor->setMachineGeometry(bounds);

  motor->CO_setNodeId(NODE_ID_LINMOT);
  motor->CO_setStatus(OD_ENTRY_H2110_linMotStatusUInt16);
  motor->CO_setMonitor(OD_ENTRY_H2114_linMotStatusSInt16);
  motor->CO_setControl(OD_ENTRY_H2111_linMotControlWord);
  motor->CO_setCmdHeader(OD_ENTRY_H2112_linMotCMD_Header);
  motor->CO_setCmdParameters(OD_ENTRY_H2113_linMotCMD_Parameters);

  engine = new StrokeEngine();
  ESP_LOGI("main", "Attaching Motor to Stroke Engine");
  engine->attachMotor(motor);

  ESP_LOGI("main", "Configuring Stroke Engine");
  engine->setParameter(StrokeParameter::PATTERN, 0);
  engine->setParameter(StrokeParameter::RATE, 50);
  engine->setParameter(StrokeParameter::DEPTH, 100);
  engine->setParameter(StrokeParameter::STROKE, 50);
  engine->setParameter(StrokeParameter::SENSATION, 0);

  if (controller != NULL) {
  ESP_LOGI("main", "Attaching Controller to Stroke Engine");
    controller->attachEngine(engine);
  }
  
  // TODO - Move controller tasks into init/attach
  if (controller != NULL) {
    controller->registerTasks();
  }

  ESP_LOGI("main", "Homing Motor");
  motor->enable();
  motor->goToHome();
  
  //ESP_LOGI("main", "Starting Motion Task!");
  //xTaskCreate(&app_motion, "app_motion", 4096, NULL, 5, NULL);
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

BLYNK_WRITE(BLYNK_DEVICE_ENABLE) {
  if (param.asInt() == 1) {
    ESP_LOGI("main", "Blynk commanded an enable!");
    if (motor->isInState(MotorState::ACTIVE) && motor->hasStatusFlag(MOTOR_FLAG_HOMED) && !engine->isActive()) {
      ESP_LOGI("main", "Motor ready and homed. Attempting to start Stroke Engine");
      engine->startPattern();
    } else {
      ESP_LOGE("main", "Motor was not ready or homed! Unable to enable device!");
      Blynk.virtualWrite(BLYNK_DEVICE_ENABLE, (float)0);
    }
  } else {
    engine->stopPattern();
  }
}
