#include <HardwareSerial.h>

#define BLYNK_TEMPLATE_ID           "TMPL000000000"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "YourAuthToken"

#define NO_GLOBAL_BLYNK

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#undef NO_GLOBAL_BLYNK

#include "config.h"
#include "blynk.hpp"
#include "StrokeEngine.h"

StrokeEngine* blynkEngine;
MotorInterface* blynkMotor;

static WiFiClient _blynkWifiClient;
static BlynkEsp32Client _blynkTransport(_blynkWifiClient);
BlynkWifi Blynk(_blynkTransport);

BlynkController::BlynkController() {
  Blynk.config(BLYNK_AUTH_TOKEN);
  while(Blynk.connect() != true) {}
}

void BlynkController::loop() {
  Blynk.run();
}

void BlynkController::attach(StrokeEngine* engine, MotorInterface* motor) {
  blynkEngine = engine;
  blynkMotor = motor;
  
  Blynk.virtualWrite(BLYNK_DEVICE_ENABLE, 0);
  engine->registerListener(this);
}

void BlynkController::engine_parameterUpdated(StrokeParameter parameter, float value) {
  switch (parameter) {
    case StrokeParameter::DEPTH:
      Blynk.virtualWrite(BLYNK_DEPTH, value);
      break;

    case StrokeParameter::STROKE:
      Blynk.virtualWrite(BLYNK_STROKE, value);
      break;

    case StrokeParameter::RATE:
      Blynk.virtualWrite(BLYNK_RATE, value);
      break;

    case StrokeParameter::SENSATION:
      Blynk.virtualWrite(BLYNK_SENSATION, value);
      break;

    case StrokeParameter::PATTERN:
      Blynk.virtualWrite(BLYNK_STROKE_TYPE, (int)value);
      break;
  }
}

BLYNK_WRITE(BLYNK_RATE) {
  blynkEngine->setParameter(StrokeParameter::RATE, param.asFloat(), true);
}

BLYNK_WRITE(BLYNK_DEPTH) {
  blynkEngine->setParameter(StrokeParameter::DEPTH, param.asFloat(), true);
}

BLYNK_WRITE(BLYNK_STROKE) {
  blynkEngine->setParameter(StrokeParameter::STROKE, param.asFloat(), true);
}

BLYNK_WRITE(BLYNK_SENSATION) {
  blynkEngine->setParameter(StrokeParameter::SENSATION, param.asFloat(), true);
} 

BLYNK_WRITE(BLYNK_STROKE_TYPE) {
  blynkEngine->setParameter(StrokeParameter::PATTERN, param.asInt(), true);
}

BLYNK_WRITE(BLYNK_DEVICE_ENABLE) {
  if (param.asInt() == 1) {
    ESP_LOGI("main", "Blynk commanded an enable!");
    if (blynkMotor->isInState(MotorState::ACTIVE) && blynkMotor->hasStatusFlag(MOTOR_FLAG_HOMED) && !blynkEngine->isActive()) {
      ESP_LOGI("main", "Motor ready and homed. Attempting to start Stroke Engine");
      blynkEngine->startPattern();
    } else {
      ESP_LOGE("main", "Motor was not ready or homed! Unable to enable device!");
      Blynk.virtualWrite(BLYNK_DEVICE_ENABLE, (float)0);
    }
  } else {
    blynkEngine->stopPattern();
  }
}