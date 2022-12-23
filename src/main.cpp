#include "esp_log.h"
#include "Wire.h"
#include <Preferences.h>

#include <Crypto.h>
#include <SHA256.h>

#include <ESPConnect.h>
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include <ESPDash.h>

#include "blynk.hpp"
#include "StrokeEngine.h"

#include "config.h"
#include "controller/canfuck.hpp"

#include "CO_main.h"
#include "motor/linmot.hpp"

#include "data_logger.hpp"

#include <Adafruit_NeoPixel.h>

#include "gui.h"


//Adafruit_NeoPixel pixels(1, 48, NEO_GRB + NEO_KHZ800);

AsyncWebServer server(80);
AsyncWebSocket ws("/");
AsyncEventSource events("/es");

// TODO - Allow ESP32 Perferences to choose motor implementation
LinmotMotor* motor;
StrokeEngine* engine;
CANFuckController* controller; // TODO - Abstract interface with controller away? Use similar system to Object Dictionary with CANOpen?
BlynkController* blynk = NULL;

ESPDash dashboard(&server); 
Card temperature(&dashboard, TEMPERATURE_CARD, "Temperature", "°C");
Card humidity(&dashboard, HUMIDITY_CARD, "Humidity", "%");

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

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      // TODO - Might support incoming data at some point, but not a concern right now
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void onConnect(AsyncEventSourceClient *client) {
    if(client->lastId()){
      Serial.printf("EventSource Client reconnected! Last message ID that it gat is: %u\n", client->lastId());
    } else {
      Serial.printf("EventSource Client connected!");
    }
    //send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!",NULL,millis(),1000);
}

void loop() {
  if (blynk != NULL) {
    blynk->loop();
  }

  // TODO - fix this stack overflow
  //ws.cleanupClients();
}

#define HASH_SIZE 32
SHA256 sha256;

void setup() {
  Serial.begin(UART_SPEED);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  const char* mac = WiFi.macAddress().c_str();
  uint8_t value[HASH_SIZE];

  Hash *hash = &sha256;
  hash->reset();
  hash->update(mac, strlen(mac));
  hash->finalize(value, sizeof(value));

  sprintf(wifiName, "CAN-SoftAP %02X", value[4]);
  sprintf(wifiPassword, "softap-%02x%02x", value[5], value[6]);

  xTaskCreatePinnedToCore(&gui_main_task, "gui", 4096*2, NULL, 0, NULL, 1);

  ESP_LOGI("main", "Wifi: %s %s", wifiName, wifiPassword);
  ESPConnect.autoConnect(wifiName, wifiPassword);

  if (ESPConnect.begin(&server)) {
    ESP_LOGI("main", "Connected to WiFi");
    //ESP_LOGI("main", "IPAddress: " + WiFi.localIP().toString());
  } else {
    ESP_LOGI("main", "Failed to connect to WiFi. Waiting for reboot...");
    while (true) { vTaskDelay(5 / portTICK_PERIOD_MS); }
  }

  ESP_LOGI("main", "Starting Bylnk!");
  //blynk = new BlynkController();

  ESP_LOGI("main", "Mounting SPIFFS");
  if(!SPIFFS.begin(true)){
    ESP_LOGE("main", "An Error has occurred while mounting SPIFFS");
    return;
  }

  ESP_LOGI("main", "Starting Web Server");
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("content-type"));

  server.serveStatic("/www2", SPIFFS, "/www/").setDefaultFile("index.html");
  server.onNotFound([](AsyncWebServerRequest *request) {
      Serial.printf("Not found: %s!\r\n", request->url().c_str());
      request->send(404);
   });

  server.begin();

  ws.onEvent(onEvent);
  events.onConnect((ArEventHandlerFunction)onConnect);
  server.addHandler(&ws);
  server.addHandler(&events);
  wlog.attachWebsocket(&ws);
  wlog.attachEventsource(&events);
  wlog.startTask();

  if (CONTROLLER_USED) {
    WEB_LOGI("main", "Initializing Hardware Controller");
    controller = new CANFuckController();
    if (controller->start() == false) {
      WEB_LOGE("main", "Unable to initialize Hardware Controller");
      esp_restart();
    }
  } else {
    WEB_LOGI("main", "No Hardware Controller, only Blynk Controls will be available!");
  }

  WEB_LOGI("main", "Configuring Motor");
  motor = new LinmotMotor();

  MachineGeometry bounds = {
    .start = 110, // mm
    .end = -7, // mm
    .keepout = 10 // mm
  };
  motor->setMaxSpeed(5000); // 5 m/s
  motor->setMaxAcceleration(500); // 25 m/s^2
  motor->setMachineGeometry(bounds);

  motor->CO_setNodeId(NODE_ID_LINMOT);
  motor->CO_setStatusUInt16(OD_ENTRY_H2110_linMotStatusUInt16);
  motor->CO_setMonitorSInt16(OD_ENTRY_H2114_linMotStatusSInt16);
  motor->CO_setMonitorSInt32(OD_ENTRY_H2115_linMotStatusSInt32);
  motor->CO_setControl(OD_ENTRY_H2111_linMotControlWord);
  motor->CO_setCmdHeader(OD_ENTRY_H2112_linMotCMD_Header);
  motor->CO_setCmdParameters(OD_ENTRY_H2113_linMotCMD_Parameters);

  engine = new StrokeEngine();
  WEB_LOGI("main", "Attaching Motor to Stroke Engine");
  engine->attachMotor(motor);

  if (controller != NULL) {
  WEB_LOGI("main", "Attaching Controller to Stroke Engine");
    controller->attachEngine(engine);
  }

  WEB_LOGI("main", "Attaching Blynk to Motor and Engine");
  blynk->attach(engine, motor);

  while (true) { vTaskDelay(5 / portTICK_PERIOD_MS); }

  WEB_LOGI("main", "Configuring Stroke Engine");
  engine->setParameter(StrokeParameter::PATTERN, 0);
  engine->setParameter(StrokeParameter::RATE, 50);
  engine->setParameter(StrokeParameter::DEPTH, 100);
  engine->setParameter(StrokeParameter::STROKE, 50);
  engine->setParameter(StrokeParameter::SENSATION, 0);
  
  // TODO - Move controller tasks into init/attach
  if (controller != NULL) {
    controller->registerTasks();
  }

  WEB_LOGI("main", "Homing Motor");
  motor->enable();
  motor->goToHome();
  
  //ESP_LOGI("main", "Starting Motion Task!");
  //xTaskCreate(&app_motion, "app_motion", 4096, NULL, 5, NULL);
}
