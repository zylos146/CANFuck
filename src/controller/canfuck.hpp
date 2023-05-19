#ifndef CONTROLLER_CANFUCK_H
#define CONTROLLER_CANFUCK_H

#include "config.h"
#include "StrokeEngine.h"

#include <SPI.h>
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>

#define SS_SWITCH        24      // this is the pin on the encoder connected to switch
#define SS_NEOPIX        6       // this is the pin on the encoder connected to neopixel

#define SEESAW_BASE_ADDR          0x36  // I2C address, starts with 0x36

#define CONTROLLER_ENCODER_COUNT 4

class CANFuckController {
  public:
    bool start();
    void attachEngine(StrokeEngine* engine) { this->engine = engine; }
    void registerTasks();
    void fetchEncoderValues();

  private:
    StrokeParameter getEncoderParameter(uint8_t num);

    StrokeEngine* engine;

    // TODO - Does not allow adjusting CONTROLLER_ENCODER_COUNT
    Adafruit_seesaw* encoders[CONTROLLER_ENCODER_COUNT];
    seesaw_NeoPixel* encoderPixels[CONTROLLER_ENCODER_COUNT];
    int32_t encoderPositions[CONTROLLER_ENCODER_COUNT] = {0, 0, 0, 0};
    bool encodersFound[CONTROLLER_ENCODER_COUNT] = {false, false, false, false};
};

#endif