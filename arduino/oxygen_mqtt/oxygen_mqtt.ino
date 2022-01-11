#include <WiFi.h>

#include <MQTTPubSubClient.h>

#include <ArduinoJson.h>

#include "MAX30100_PulseOximeter.h"

WiFiClient client;
MQTTPubSubClient mqtt;
DynamicJsonDocument doc(200);
PulseOximeter pox;

const char * ssid = "XVALEE";
const char * pass = "va6333le";
const char * controlTopic = "control";
const char * biomedicalTopic = "biomedical";
int stress;
int stressSensor = A0;
bool pump;
char buf[40];

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // connecting to wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("WiFi connected!");

  // connecting to host
  while (!client.connect("192.168.0.200", 1883)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("host connected!");

  // initialize mqtt client
  mqtt.begin(client);

  Serial.print("connecting to mqtt broker...");
  while (!mqtt.connect("arduino", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" connected!");

  // subscribe topic and callback which is called when test2 has come
  mqtt.subscribe(controlTopic, [](const String & payload,
    const size_t size) {
    deserializeJson(doc, payload);
    pump = doc["pump"];
    stress = doc["stress"];
    // Serial.println(doc["pump"].as<bool>());
    // serializeJson(doc, Serial);
  });

  // initial pump pin output
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);

  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  // Register a callback for the beat detection
  // pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update(); // Make sure to call update as fast as possible
  mqtt.update(); // should be called

  static uint32_t prev_ms = millis();
  if (millis() > prev_ms + 1000) {
    // Asynchronously publish heart rate and oxidation
    sprintf(buf, "{\"oxygen\": %d, \"heartRate\": %d}", pox.getSpO2(), pox.getHeartRate());
    mqtt.publish(biomedicalTopic, buf);

    // calibration stress sensor data
    stressSensorRead = analogRead(stressSensor) - 548) * 37.5;
  }
}