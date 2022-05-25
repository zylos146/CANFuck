#include "controller.hpp"

#define ENCODER_1_PARAMETER StrokeParameter::DEPTH
#define ENCODER_2_PARAMETER StrokeParameter::STROKE
#define ENCODER_3_PARAMETER StrokeParameter::RATE
#define ENCODER_4_PARAMETER c::SENSATION

void controller_poll(void* pvParameter) {
  Controller* controller = (Controller*)pvParameter;
  while (true) {
    controller->fetchEncoderValues();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

StrokeParameter Controller::getEncoderParameter(uint8_t num) {
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

void Controller::fetchEncoderValues() {
  for (uint8_t enc=0; enc < CONTROLLER_ENCODER_COUNT; enc++) {
    this->engine->setParameter(this->getEncoderParameter(enc), this->encoders[enc]->getEncoderPosition());
  }
}

bool Controller::start() {
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

void Controller::registerTasks() {
  ESP_LOGI("controller", "Registering tasks");
  xTaskCreate(&controller_poll, "app_motion", 4096, this, 5, NULL);
}