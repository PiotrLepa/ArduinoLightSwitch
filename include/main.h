#ifndef MAIN_H

#include <Arduino.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "smart-home-3d433.firebaseio.com"
#define FIREBASE_AUTH "ikxJKJ8FDoSkZ5rbwM2YLuPQ6coBJ8a60jZbtdRO"

#define WIFI_SSID "WIFI_SSID"
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
