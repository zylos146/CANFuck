#include "controller/canfuck.hpp"

#define ENCODER_1_PARAMETER StrokeParameter::DEPTH
#define ENCODER_2_PARAMETER StrokeParameter::STROKE
#define ENCODER_3_PARAMETER StrokeParameter::RATE
#define ENCODER_4_PARAMETER StrokeParameter::SENSATION

void controller_poll(void* pvParameter) {
  CANFuckController* controller = (CANFuckController*)pvParameter;
  while (true) {
    controller->fetchEncoderValues();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

StrokeParameter CANFuckController::getEncoderParameter(uint8_t num) {
  switch (num) {
    case 1:
      return StrokeParameter::DEPTH;
    case 2:
      return StrokeParameter::STROKE;
    case 3:
      return StrokeParameter::RATE;
    case 4:
      return StrokeParameter::SENSATION;
  }
}

// TODO - Need to use delta readings so we can sync Blynk and Hardware controller?
// Really, StrokeEngine is the master parameter holder here
// TODO - This needs to check delta and only report if there was a change
// TODO - need to adjust delta and scale based on reported Parameter range from Engine
void CANFuckController::fetchEncoderValues() {
  for (uint8_t enc=0; enc < CONTROLLER_ENCODER_COUNT; enc++) {
    Adafruit_seesaw* encoder = this->encoders[enc];
    seesaw_NeoPixel* pixel = this->encoderPixels[enc];

    bool active = encoder->digitalRead(SS_SWITCH);
    
    StrokeParameter parameter = this->getEncoderParameter(enc);
    int32_t delta = encoder->getEncoderDelta();

    if (delta != 0) {
      float value = this->engine->getParameter(parameter);
      this->engine->setParameter(parameter, value + delta, false);
    }

    if (active) {
      pixel->setBrightness(10);
      pixel->setPixelColor(0, pixel->Color(255, 165, 0));
      pixel->show();
    } else {
      pixel->setBrightness(10);
      pixel->setPixelColor(0, pixel->Color(0, 255, 0));
      pixel->show();
    }
  }
}

bool CANFuckController::start() {
  ESP_LOGI("controller", "Attempting to register %d encoders!", CONTROLLER_ENCODER_COUNT);
  bool encoderSuccess = true;

  for (uint8_t enc=0; enc < CONTROLLER_ENCODER_COUNT; enc++) {
    this->encoders[enc] = new Adafruit_seesaw();
    this->encoderPixels[enc] = new seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800);
    
    Adafruit_seesaw* encoder = this->encoders[enc];
    seesaw_NeoPixel* pixel = this->encoderPixels[enc];

    if (
      !encoder->begin(SEESAW_BASE_ADDR + enc) ||
      !pixel->begin(SEESAW_BASE_ADDR + enc)
    ) {
      ESP_LOGE("controller", "Couldn't find encoder: %d", enc);
      encoderSuccess = false;
      break;
    }

    ESP_LOGI("controller", "Found Encoder + Pixel: %d", enc);
    
    uint32_t version = ((encoder->getVersion() >> 16) & 0xFFFF);
    if (version != 4991){
      ESP_LOGE("controller", "Couldn't find encoder: %d", enc);
      encoderSuccess = false;
      break;
    }
    ESP_LOGI("controller", "Verified Encoder %d has firmware 4991 loaded!", enc);

    // use a pin for the built in encoder switch
    encoder->pinMode(SS_SWITCH, INPUT_PULLUP);

    // get starting position
    this->encoderPositions[enc] = encoder->getEncoderPosition();

    delay(10);
    encoder->setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
    encoder->enableEncoderInterrupt();
    
    // set not so bright!
    pixel->setBrightness(10);
    pixel->setPixelColor(0, pixel->Color(255, 0, 127));
    pixel->show();
  }

  if (encoderSuccess == false) {
    ESP_LOGE("controller", "Unable to initialize all encoders!");
    return false;
  }

  return true;
}

void CANFuckController::registerTasks() {
  ESP_LOGI("controller", "Registering tasks");
  xTaskCreate(&controller_poll, "app_motion", 4096, this, 5, NULL);
}