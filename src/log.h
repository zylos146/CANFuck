#include "config.h"

#if FRAMEWORK_USED == FRAMEWORK_ESP_IDF
  #include "esp_err.h"
  #include "esp_log.h"

  void log(char* message) {
    
  }
#else

  #include <Arduino.h>
  void log(char* message) {
    Serial.println(message);
  }
#endif

// TODO - Pull these log levels from ESP_IDF existing log defines?
#define LOG_DEBUG 5
#define LOG_INFO 4
#define LOG_WARN 3
#define LOG_ERROR 2
#define LOG_FATAL 1
#define LOG_LEVEL LOG_DEBUG

#if LOG_LEVEL == LOG_DEBUG
  void log_debug(char* message) {
    log(message);
  };
#else
  void log_debug(char* message) {};
#endif

#if LOG_LEVEL >= LOG_INFO
  void log_info(char* message) {
    log(message);
  };
#else
  void log_info(char* message) {};
#endif

#if LOG_LEVEL >= LOG_WARN
  void log_warn(char* message) {
    log(message);
  };
#else
  void log_warn(char* message) {};
#endif