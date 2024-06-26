/**
 *   ESP32 SvelteKit
 *
 *   A simple, secure and extensible framework for IoT projects for ESP32 platforms
 *   with responsive Sveltekit front-end built with TailwindCSS and DaisyUI.
 *   https://github.com/theelims/ESP32-sveltekit
 *
 *   Copyright (C) 2018 - 2023 rjwats
 *   Copyright (C) 2023 theelims
 *
 *   All Rights Reserved. This software may be modified and distributed under
 *   the terms of the LGPL v3 license. See the LICENSE file for details.
 **/

#include <ESP32SvelteKit.h>
#include <service/SerialStateService.hpp>
#include <service/MachineStateService.hpp>
#include <service/StrokeStateService.hpp>
#include <service/LoggingService.hpp>

#include <quickjs.h>
#include "quickjs-libc.h"

AsyncWebServer server(80);
ESP32SvelteKit esp32sveltekit(&server);
SerialStateService serialState(&server, esp32sveltekit.getSecurityManager(), esp32sveltekit.getFS());
MachineStateService machineState(&server, esp32sveltekit.getSecurityManager(), esp32sveltekit.getFS());
StrokeStateService strokeState(&server, esp32sveltekit.getSecurityManager());
LoggingStateService loggingState(&server, esp32sveltekit.getSecurityManager());

JSContext *js_ctx;
void setup_js() {
  JSRuntime *rt = JS_NewRuntime();
  if (!rt) {
    return;
  }

  // TODO - These memory limits are absurd
  // See if malloc in quickjs can be swapped for PSRAM malloc to give is breathing room
  JS_SetMemoryLimit(rt, 64 * 1024 * 1024);
  JS_SetGCThreshold(rt, 16 * 1024 * 1024);

  js_ctx = JS_NewContext(rt);
  if (!js_ctx) {
      JS_FreeRuntime(rt);
      return;
  }
}

void setup()
{
  pinMode(CAN_STANDBY_IO, OUTPUT);
  digitalWrite(CAN_STANDBY_IO, LOW);
  
  Serial.begin(115200);
  loggingState.begin();
  server.begin();

  esp32sveltekit.setMDNSAppName("canfuck");
  esp32sveltekit.begin();
  serialState.begin();
  machineState.begin();
  strokeState.begin(&machineState); // Init StrokeEngine

  //setup_js();
}

void loop() { esp32sveltekit.loop(); }
