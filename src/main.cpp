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

AsyncWebServer server(80);
ESP32SvelteKit esp32sveltekit(&server);
SerialStateService serialState(&server, esp32sveltekit.getSecurityManager(), esp32sveltekit.getFS());

void setup()
{
  Serial.begin(115200);

  esp32sveltekit.setMDNSAppName("canfuck");
  esp32sveltekit.begin();
  serialState.begin();
  server.begin();
}

void loop()
{
  esp32sveltekit.loop();
}
