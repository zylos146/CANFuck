#pragma once

#include "WiFi.h"
#include "AsyncTCP.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <functional>
#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>
#include <FSPersistence.h>

// GET /rest/machine/config
// POST /rest/machine/config

// POST /rest/machine/home
// POST /rest/machine/

// GET /rest/stroke
//
// /ws/stroke

#define SERIAL_STATUS_ENDPOINT_PATH "/rest/serial/status"
#define SERIAL_SETTINGS_ENDPOINT_PATH "/rest/serial/settings"

class SerialState
{
public:
  boolean enabled = true;
  unsigned long baudrate = 38400;
  uint32_t rs_config = SERIAL_8N1; // TODO - Allow to be used in string form in UI
  uint16_t port = 23;
  int8_t rx_pin = 18;
  int8_t tx_pin = 17;

  static void read(SerialState &settings, JsonObject &root)
  {
    root["enabled"] = settings.enabled; // TODO - Tie into actually turning server on/off
    root["baudrate"] = settings.baudrate;
    root["rs_config"] = settings.rs_config;
    root["port"] = settings.port;
    root["rx_pin"] = settings.rx_pin;
    root["tx_pin"] = settings.tx_pin;
  }

  static StateUpdateResult update(JsonObject &root, SerialState &state)
  {
    state.enabled = root["enabled"];
    state.baudrate = root["baudrate"];
    state.rs_config = root["rs_config"];
    state.port = root["port"];
    state.rx_pin = root["rx_pin"];
    state.tx_pin = root["tx_pin"];
    
    return StateUpdateResult::CHANGED;
  }
};

class SerialStateService : public StatefulService<SerialState>
{
public:
  // SerialStateService(AsyncWebServer *server,
  //                   SecurityManager *securityManager);

  SerialStateService(AsyncWebServer *server, SecurityManager *securityManager, FS* fs) : 
    _httpEndpoint(
      SerialState::read,
      SerialState::update,
      this,
      server,
      SERIAL_SETTINGS_ENDPOINT_PATH,
      securityManager,
      AuthenticationPredicates::IS_AUTHENTICATED
    ),
    _fsPersistence(
      SerialState::read, 
      SerialState::update, 
      this, 
      fs, 
      "/config/serial.json"
    )
  {
    server->on(
        SERIAL_STATUS_ENDPOINT_PATH,
        HTTP_GET,
        securityManager->wrapRequest(
            std::bind(&SerialStateService::getStatus, this, std::placeholders::_1),
            AuthenticationPredicates::IS_ADMIN));
  }

  void getStatus(AsyncWebServerRequest *request)
  {
      AsyncJsonResponse *response = new AsyncJsonResponse(false, DEFAULT_BUFFER_SIZE);
      JsonObject jsonObject = response->getRoot().to<JsonObject>();

      jsonObject["available"] = activeClient == NULL;
      jsonObject["connected_ip"] = "";

      if (activeClient != NULL) {
        jsonObject["connected_ip"] = activeClient->remoteIP().toString();
      }

      response->setLength();
      request->send(response);
  }

  void begin() {
    _fsPersistence.readFromFS();
    read([&](SerialState state){ 
      Serial2.begin(state.baudrate, state.rs_config, SERIAL_RX_PIN, SERIAL_TX_PIN);
      
      server = new AsyncServer(state.port);
      server->onClient(std::bind(&SerialStateService::handleNewClient, this, std::placeholders::_1, std::placeholders::_2), server);
      server->begin();

      xTaskCreatePinnedToCore(&SerialStateService::handlePoll, "serial_task", 4096, this, 5, NULL, 1);
    });
  }

  void handleData(void *arg, AsyncClient *client, void *data, size_t len)
  {
    array_to_string((byte*)data, len, hexBuffer);
    log_i("data received from client %s \n", client->remoteIP().toString().c_str());
    log_i("Client Data %u: %s", len, hexBuffer);
    
    Serial2.write((char*)data, len);
    Serial2.flush();
  }

  void handleError(void *arg, AsyncClient *client, int8_t error)
  {
    activeClient = NULL;
    log_i("connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
  }

  void handleDisconnect(void *arg, AsyncClient *client)
  {
    activeClient = NULL;
    log_i("client %s disconnected\n", client->remoteIP().toString().c_str());
  }

  void handleTimeOut(void *arg, AsyncClient *client, uint32_t time)
  {
    activeClient = NULL;
    log_i("client %s ACK timeout\n", client->remoteIP().toString().c_str());
  }

  // TODO - really need some locking around here
  void handleNewClient(void* arg, AsyncClient* client) {
    if (activeClient != NULL) {
      log_w(
        "active client %s already exists, new client %s will be rejected", 
        activeClient->remoteIP().toString().c_str(), 
        client->remoteIP().toString().c_str()
      );

      client->stop();
      return;
    }

    read([&](SerialState state){ 
      if (!state.enabled) {
        log_w(
          "Remote Serial is disabled. New client %s will be rejected!", 
          activeClient->remoteIP().toString().c_str(), 
          client->remoteIP().toString().c_str()
        );

        client->stop();
        activeClient = NULL;
        return;
      }

      log_i("new client %s has connected to server", client->remoteIP().toString().c_str());

      activeClient = client;
      client->onData        (std::bind(&SerialStateService::handleData,       this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), NULL);
      client->onError       (std::bind(&SerialStateService::handleError,      this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), NULL);
      client->onDisconnect  (std::bind(&SerialStateService::handleDisconnect, this, std::placeholders::_1, std::placeholders::_2), NULL);
      client->onTimeout     (std::bind(&SerialStateService::handleTimeOut,    this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), NULL);
    });
  }

  // TODO - Probably some sync locks here?
  static void handlePoll(void* pvParameter) {
    SerialStateService* service = (SerialStateService*)pvParameter;
    size_t size = 0;

    while (true) {
      if (service->activeClient != NULL && service->activeClient->canSend()) {
        while ((size = Serial2.available())) {
          size = (size >= SERIAL_BUFFER_SIZE ? SERIAL_BUFFER_SIZE : size);
          Serial2.readBytes((char*)service->buffer, size);
          service->activeClient->add((const char*)service->buffer, size);
          service->activeClient->send();

          array_to_string((byte*)service->buffer, size, service->hexBuffer);
          log_i("Server Data %u: %s", size, service->hexBuffer);

          vTaskDelay(1 / portTICK_PERIOD_MS); 
        }
      }

      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
  }

  // TODO - Handle update by shutting down and re-booting server

private:
  HttpEndpoint<SerialState> _httpEndpoint;
  FSPersistence<SerialState> _fsPersistence;

  AsyncServer* server = NULL;
  AsyncClient* activeClient = NULL;
  char buffer[SERIAL_BUFFER_SIZE];
  char hexBuffer[SERIAL_BUFFER_SIZE];

  // https://stackoverflow.com/a/44749986
  static void array_to_string(byte array[], unsigned int len, char buffer[])
  {
      for (unsigned int i = 0; i < len; i++)
      {
          byte nib1 = (array[i] >> 4) & 0x0F;
          byte nib2 = (array[i] >> 0) & 0x0F;
          buffer[i*3+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
          buffer[i*3+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
          buffer[i*3+2] = ' ';
      }
      buffer[len*3] = '\0';
  }
};