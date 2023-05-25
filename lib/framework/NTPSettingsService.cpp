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

#include <NTPSettingsService.h>

NTPSettingsService::NTPSettingsService(AsyncWebServer *server, FS *fs, SecurityManager *securityManager) : _httpEndpoint(NTPSettings::read, NTPSettings::update, this, server, NTP_SETTINGS_SERVICE_PATH, securityManager),
                                                                                                           _fsPersistence(NTPSettings::read, NTPSettings::update, this, fs, NTP_SETTINGS_FILE),
                                                                                                           _timeHandler(TIME_PATH,
                                                                                                                        securityManager->wrapCallback(
                                                                                                                            std::bind(&NTPSettingsService::configureTime, this, std::placeholders::_1, std::placeholders::_2),
                                                                                                                            AuthenticationPredicates::IS_ADMIN))
{
    _timeHandler.setMethod(HTTP_POST);
    _timeHandler.setMaxContentLength(MAX_TIME_SIZE);
    server->addHandler(&_timeHandler);
    WiFi.onEvent(
        std::bind(&NTPSettingsService::onStationModeDisconnected, this, std::placeholders::_1, std::placeholders::_2),
        WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.onEvent(std::bind(&NTPSettingsService::onStationModeGotIP, this, std::placeholders::_1, std::placeholders::_2),
                 WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    addUpdateHandler([&](const String &originId)
                     { configureNTP(); },
                     false);
}

void NTPSettingsService::begin()
{
    _fsPersistence.readFromFS();
    configureNTP();
}

void NTPSettingsService::onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println(F("Got IP address, starting NTP Synchronization"));
    configureNTP();
}

void NTPSettingsService::onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println(F("WiFi connection dropped, stopping NTP."));
    configureNTP();
}

void NTPSettingsService::configureNTP()
{
    if (WiFi.isConnected() && _state.enabled)
    {
        Serial.println(F("Starting NTP..."));
        configTzTime(_state.tzFormat.c_str(), _state.server.c_str());
    }
    else
    {
        setenv("TZ", _state.tzFormat.c_str(), 1);
        tzset();
        sntp_stop();
    }
}

void NTPSettingsService::configureTime(AsyncWebServerRequest *request, JsonVariant &json)
{
    if (!sntp_enabled() && json.is<JsonObject>())
    {
        struct tm tm = {0};
        String timeLocal = json["local_time"];
        char *s = strptime(timeLocal.c_str(), "%Y-%m-%dT%H:%M:%S", &tm);
        if (s != nullptr)
        {
            time_t time = mktime(&tm);
            struct timeval now = {.tv_sec = time};
            settimeofday(&now, nullptr);
            AsyncWebServerResponse *response = request->beginResponse(200);
            request->send(response);
            return;
        }
    }
    AsyncWebServerResponse *response = request->beginResponse(400);
    request->send(response);
}
