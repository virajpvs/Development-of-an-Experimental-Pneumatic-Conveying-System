/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-websocket-server-sensor/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include "HX710B.h"

const int DOUT = 21;   //sensor data pin
const int SCLK  = 22;   //sensor clock pin
float Reading = 0;

HX710B pressure_sensor;

// Replace with your network credentials
const char* ssid = "HUAWEI-03AF";
const char* password =  "983180930v";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 100;

float ReadPressure1() {
  digitalWrite(15,LOW);
  digitalWrite(2,LOW);
  digitalWrite(4,LOW);
  Reading = pressure_sensor.pascal();
  Serial.println(Reading);
  return Reading;
}
float ReadPressure2() {
  digitalWrite(15,HIGH);
  digitalWrite(2,LOW);
  digitalWrite(4,LOW);
  Reading = pressure_sensor.pascal();
  Serial.println(Reading);
  return Reading;
}
float ReadPressure3() {
  digitalWrite(15,LOW);
  digitalWrite(2,HIGH);
  digitalWrite(4,LOW);
  Reading = pressure_sensor.pascal();
  Serial.println(Reading);
  return Reading;
}

// Get Sensor Readings and return JSON object
String getSensorReadings(){
  readings["temperature"] = String(ReadPressure1());
  delay(100);
  readings["humidity"] =  String(ReadPressure2());
  delay(100);
  readings["pressure"] = String(ReadPressure3());
  delay(100);
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void notifyClients(String sensorReadings) {
  ws.textAll(sensorReadings);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    //data[len] = 0;
    //String message = (char*)data;
    // Check if the message is "getReadings"
    //if (strcmp((char*)data, "getReadings") == 0) {
      //if it is, send current sensor readings
      String sensorReadings = getSensorReadings();
      Serial.print(sensorReadings);
      notifyClients(sensorReadings);
    //}
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
  Serial.begin(115200);
  
  initWiFi();
  initSPIFFS();
  initWebSocket();
  
  pinMode(15, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);


  pressure_sensor.begin(DOUT, SCLK);

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  // Start server
  server.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    String sensorReadings = getSensorReadings();
    Serial.print(sensorReadings);
    notifyClients(sensorReadings);

  lastTime = millis();

  }

  ws.cleanupClients();
}
