#include "main.h"

//Define FirebaseESP8266 data object
FirebaseData firebaseDataRead;
FirebaseData firebaseDataWrite;

String firebasePath = "/rooms/my_room/light";
String lightControlPath = firebasePath + "/enabled";

int lighFirebaseState = LOW;
int lightSwitchState = LOW;
int previuosLightSwitchState = LOW;

int inputCount = 0;

unsigned long readInputDelayMillis = 0;

void setup() {
  Serial.begin(9600);
  setupPins();
  connectToWifi();
  setupFirebase();
}

void loop() {
  if (millis() - readInputDelayMillis > 100) {
    readInputDelayMillis = millis();

    int currentLightSwitchState = !digitalRead(LIGHT_SWITCH_INPUT);

    if (currentLightSwitchState == previuosLightSwitchState) {
      inputCount++;
    } else {
      inputCount = 0;
    }
    previuosLightSwitchState = currentLightSwitchState;

    if (lightSwitchState != currentLightSwitchState && inputCount >= 3) {
      inputCount = 0;
      lightSwitchState = currentLightSwitchState;
      saveLightStateToFirebase();
    }
  }
}

void saveLightStateToFirebase() {
  if (Firebase.setBool(firebaseDataWrite, lightControlPath, !lighFirebaseState)) {
    Serial.println("------------------------------------");
    Serial.println("SAVED LIGHT STATUS");
    Serial.print("VALUE: ");
    Serial.println(firebaseDataWrite.boolData() == 1 ? "true" : "false");
  } else {
    Serial.println("FAILED SAVING TO FIREBASE - REASON: " + firebaseDataWrite.errorReason());
  }
}

void streamCallback(StreamData data) {
  Serial.println("------------------------------------");
  Serial.println("LIGHT STATUS CHANGED...");
  Serial.println("STREAM PATH: " + data.streamPath());
  if (data.dataType() == "boolean") {
    lighFirebaseState = data.boolData();

    Serial.print("VALUE: ");
    Serial.println(data.boolData() == 1 ? "true" : "false");

    setLightState(lighFirebaseState);
  }
}

void setLightState(bool enabled) {
  digitalWrite(LIGHT_OUTPUT, enabled);
}

void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("\nStream timeout, resume streaming...\n");
  }
}

void setupFirebase() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  if (!Firebase.beginStream(firebaseDataRead, firebasePath)) {
    Serial.println("------------------------------------");
    Serial.println("Can't begin stream connection - REASON: " + firebaseDataRead.errorReason());
  }

  Firebase.setStreamCallback(firebaseDataRead, streamCallback, streamTimeoutCallback);
  Firebase.setBool(firebaseDataWrite, lightControlPath, false);
}

void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.print("\nConnected with IP: ");
  Serial.println(WiFi.localIP());
}

void setupPins() {
  pinMode(LIGHT_SWITCH_INPUT, INPUT);
  pinMode(LIGHT_OUTPUT, OUTPUT);
}
