#ifndef MAIN_H

#include <Arduino.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "FIREBASE_HOST"
#define FIREBASE_AUTH "FIREBASE_AUTH"

#define WIFI_SSID "HUAWEI-B315-WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define LIGHT_SWITCH_INPUT D2
#define LIGHT_OUTPUT D6

void setupPins();
void connectToWifi();
void setupFirebase();
void saveLightStateToFirebase();
void streamCallback(StreamData data);
void streamTimeoutCallback(bool timeout);
void setLightState(bool lightEnabled);

#endif
