/*
 * WebSocketClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Arduino_JSON.h>

#define SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

const char *HOST = "192.168.1.2";
const int PORT = 3000;
const char *WIFI_SSID = "WE_7F8408";
const char *WIFI_PASS = "passWord_123";

int level = 0;

void updateState(uint8_t *payload) {
  JSONVar newState = JSON.parse((char *)payload);
  // JSON.typeof(jsonVar) can be used to get the type of the variable
  if (JSON.typeof(newState) == "undefined") {
    SERIAL.println("error: parsing input failed!");
    return;
  }

  if (!newState.hasOwnProperty("level")) {
    SERIAL.println("error: payload doesn't contain level property");
    return;
  }

  int newLevel = (int)newState["level"];

  SERIAL.printf("new state ==> level = %d\n", newLevel);
  if (newLevel > 100 || newLevel < 0) {
    SERIAL.println("error: invalid level");
    return;
  }

  level = newLevel;
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      SERIAL.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      SERIAL.printf("[WSc] Connected to url: %s\n", payload);
      break;
    case WStype_PING:
      // pong will be send automatically
      SERIAL.printf("[WSc] get ping\n");
      break;
    case WStype_PONG:
      // answer to a ping we send
      SERIAL.printf("[WSc] get pong\n");
      break;
    case WStype_TEXT:
      SERIAL.printf("[WSc] get text: %s\n", payload);
      updateState(payload);
      // send message to server
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      SERIAL.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);
      // send data to server
      // webSocket.sendBIN(payload, length);
      break;
  }
}

void setup() {
  // LED output pin
  pinMode(D1, OUTPUT);
  setupSerial();
  setupWifi();
  setupWebSocket();
}

void setupSerial() {
  // SERIAL.begin(921600);
  SERIAL.begin(115200);
  while (!SERIAL);

  SERIAL.setDebugOutput(true);

  SERIAL.println();
  SERIAL.println();
  SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    SERIAL.flush();
    delay(1000);
  }
}

void setupWifi() {
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASS);

  //WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }
}

void setupWebSocket() {
  // server address, port and URL
  webSocket.begin(HOST, PORT, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  // webSocket.setAuthorization("user", "Password");

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);

  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  webSocket.enableHeartbeat(15000, 3000, 2);
}

void loop() {
  webSocket.loop();
  analogWrite(D1, 1.0 * level / 100 * 255);
}
