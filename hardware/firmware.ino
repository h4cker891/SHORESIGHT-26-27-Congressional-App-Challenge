#include <WiFi.h>
#include <WebServer.h>


const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";


const int TURBIDITY_PIN = 34;
const float ADC_REFERENCE_V = 3.3f;
const float ADC_MAX_COUNT = 4095.0f;
const float DIVIDER_RECONSTRUCTION = 1.5f;

WebServer server(80);

void sendReadings() {
  const int raw = analogRead(TURBIDITY_PIN);
  const float adcVoltage = raw * (ADC_REFERENCE_V / ADC_MAX_COUNT);
  const float sensorVoltage = adcVoltage * DIVIDER_RECONSTRUCTION;

  String json = "{\"raw\":" + String(raw) +
                ",\"voltage\":" + String(sensorVoltage, 3) + "}";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("ShoreSight device address: http://");
  Serial.println(WiFi.localIP());

  server.on("/api/readings", HTTP_GET, sendReadings);
  server.begin();
}

void loop() {
  server.handleClient();
}
