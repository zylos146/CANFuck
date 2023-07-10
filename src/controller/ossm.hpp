#ifndef CONTROLLER_OSSM_H
#define CONTROLLER_OSSM_H


#include "StrokeEngine.h"

#include <SPI.h>
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>

/*These are configured for the OSSM Remote - which has a screen, a potentiometer and an encoder which clicks*/
#define LOCAL_CONTROLLER INPUT_PULLDOWN
#define SPEED_POT_PIN 34
#define ENCODER_SWITCH 35
#define ENCODER_A 18
#define ENCODER_B 5
#define REMOTE_SDA 21
#define REMOTE_CLK 19
#define REMOTE_ADDRESS 0x3c

class OSSMController {
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