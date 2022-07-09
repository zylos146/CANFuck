#ifndef BLYNK_INCLUDES_H
#define BLYNK_INCLUDES_H

#include <HardwareSerial.h>

#define BLYNK_TEMPLATE_ID           "TMPL4LnmPRGq"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "PNj6yEcR6u9ksT6IuUZFUY4yqpjtTAcX"


#define NO_GLOBAL_BLYNK

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#undef NO_GLOBAL_BLYNK

#define BLYNK_LINMOT_STATUS_PIN 0
#define BLYNK_LINMOT_STATE_PIN 1
#define BLYNK_LINMOT_ACTUAL_POSITION 4
#define BLYNK_LINMOT_DEMAND_CURRENT 5
#define BLYNK_LINMOT_DEMAND_POSITION 11
#define BLYNK_LINMOT_MODEL_TEMP 12
#define BLYNK_LINMOT_REAL_TEMP 13
#define BLYNK_LINMOT_MOTOR_VOLTAGE 14
#define BLYNK_LINMOT_POWER_LOSS 15
#define BLYNK_DEVICE_ENABLE 16

#define BLYNK_STATE_PIN 2
#define BLYNK_STATUS_PIN 3

#define BLYNK_DEPTH 6
#define BLYNK_STROKE 7
#define BLYNK_SPEED 8
#define BLYNK_SENSATION 9

#define BLYNK_LOG 10

#endif