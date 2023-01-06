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
#include "motor/virtual.hpp"
#include "lvgl_gui.hpp"
#include "screen/boot.hpp"
#include "screen/status.hpp"
#include "screen/wifi.hpp"

#include "data_logger.hpp"
#include "serial.hpp"
#include "wifi.hpp"

#include <Adafruit_NeoPixel.h>
#include "esp32s3/rom/rtc.h"


//Adafruit_NeoPixel pixels(1, 48, NEO_GRB + NEO_KHZ800);

AsyncWebServer server(80);
AsyncWebSocket ws("/");
AsyncEventSource events("/es");

// TODO - Allow ESP32 Perferences to choose motor implementation

#if MOTOR_USED == MOTOR_LINMOT
  LinmotMotor* motor;
#elif MOTOR_USED == MOTOR_MOCK
  VirtualMotor* motor;
#endif

StrokeEngine* engine;
CANFuckController* controller; // TODO - Abstract interface with controller away? Use similar system to Object Dictionary with CANOpen?
BlynkController* blynk = NULL;

//ESPDash dashboard(&server); 
//Card temperature(&dashboard, TEMPERATURE_CARD, "Temperature", "°C");
//Card humidity(&dashboard, HUMIDITY_CARD, "Humidity", "%");

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

LVGLGui* gui;

void print_reset_reason(int reason)
{
  switch ( reason)
  {
    case 1 : ESP_LOGI("main", "POWERON_RESET");break;          /**<1,  Vbat power on reset*/
    case 3 : ESP_LOGI("main", "SW_RESET");break;               /**<3,  Software reset digital core*/
    case 4 : ESP_LOGI("main", "OWDT_RESET");break;             /**<4,  Legacy watch dog reset digital core*/
    case 5 : ESP_LOGI("main", "DEEPSLEEP_RESET");break;        /**<5,  Deep Sleep reset digital core*/
    case 6 : ESP_LOGI("main", "SDIO_RESET");break;             /**<6,  Reset by SLC module, reset digital core*/
    case 7 : ESP_LOGI("main", "TG0WDT_SYS_RESET");break;       /**<7,  Timer Group0 Watch dog reset digital core*/
    case 8 : ESP_LOGI("main", "TG1WDT_SYS_RESET");break;       /**<8,  Timer Group1 Watch dog reset digital core*/
    case 9 : ESP_LOGI("main", "RTCWDT_SYS_RESET");break;       /**<9,  RTC Watch dog Reset digital core*/
    case 10 : ESP_LOGI("main", "INTRUSION_RESET");break;       /**<10, Instrusion tested to reset CPU*/
    case 11 : ESP_LOGI("main", "TGWDT_CPU_RESET");break;       /**<11, Time Group reset CPU*/
    case 12 : ESP_LOGI("main", "SW_CPU_RESET");break;          /**<12, Software reset CPU*/
    case 13 : ESP_LOGI("main", "RTCWDT_CPU_RESET");break;      /**<13, RTC Watch dog Reset CPU*/
    case 14 : ESP_LOGI("main", "EXT_CPU_RESET");break;         /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : ESP_LOGI("main", "RTCWDT_BROWN_OUT_RESET");break;/**<15, Reset when the vdd voltage is not stable*/
    case 16 : ESP_LOGI("main", "RTCWDT_RTC_RESET");break;      /**<16, RTC Watch dog reset digital core and rtc module*/
    default : ESP_LOGI("main", "NO_MEAN");
  }
}

void setup() {
  // Disables serial output basically except over USB
  Serial.begin(115200, SERIAL_8N1, 45, 48);

  // Wait for initial monitor to connect
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  ESP_LOGI("main", "CPU0 reset reason:");
  print_reset_reason(rtc_get_reset_reason(0));

  ESP_LOGI("main", "CPU1 reset reason:");
  print_reset_reason(rtc_get_reset_reason(1));

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  const char* mac = WiFi.macAddress().c_str();
  uint8_t value[HASH_SIZE];

  Hash *hash = &sha256;
  hash->reset();
  hash->update(mac, strlen(mac));
  hash->finalize(value, sizeof(value));

  sprintf(wifiName, "CAN-SoftAP %02X", value[4]);
  sprintf(wifiPassword, "softap-%02x%02x", value[5], value[6]);

  //gui = new LVGLGui();
  //gui->start();

  //BootScreen* bootScreen = new BootScreen();
  //StatusScreen* statusScreen = new StatusScreen();
  //WifiScreen* wifiScreen = new WifiScreen();
  //gui->activate(wifiScreen);

  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin("Central Nexus", "taobao123");
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      vTaskDelay(5 / portTICK_PERIOD_MS);
  }
  
  ESP_LOGI("main", "Wifi: %s %s", wifiName, wifiPassword);
  ESPConnect.autoConnect(wifiName, "test1234");

  if (ESPConnect.begin(&server)) {
    ESP_LOGI("main", "Connected to WiFi");
    //ESP_LOGI("main", "IPAddress: " + WiFi.localIP().toString());
  } else {
    ESP_LOGI("main", "Failed to connect to WiFi. Waiting for reboot...");
    while (true) { vTaskDelay(5 / portTICK_PERIOD_MS); }
  }
  
  //gui->activate(bootScreen);
  vTaskDelay(5000 / portTICK_PERIOD_MS);

  //gui->activate(statusScreen);

  //ESP_LOGI("main", "Starting Bylnk!");
  //blynk = new BlynkController();
  serial_setup();

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
  //wlog.attachWebsocket(&ws);
  //wlog.attachEventsource(&events);
  //wlog.startTask();

  while (true) {
    vTaskDelay(500 / portTICK_PERIOD_MS); 
  }

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

#if MOTOR_USED == MOTOR_LINMOT
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
#elif MOTOR_USED == MOTOR_MOCK
  motor = new VirtualMotor();

  MachineGeometry bounds = {
    .start = 0, // mm
    .end = 100, // mm
    .keepout = 10 // mm
  };
  motor->setMaxSpeed(5000); // 5 m/s
  motor->setMaxAcceleration(500); // 25 m/s^2
  motor->setMachineGeometry(bounds);
#endif

  engine = new StrokeEngine();
  WEB_LOGI("main", "Attaching Motor to Stroke Engine");
  engine->attachMotor(motor);

  if (controller != NULL) {
  WEB_LOGI("main", "Attaching Controller to Stroke Engine");
    controller->attachEngine(engine);
  }

  WEB_LOGI("main", "Attaching Blynk to Motor and Engine");
  blynk->attach(engine, motor);

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
}
