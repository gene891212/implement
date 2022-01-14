#include <WiFi.h>

#include <MQTTPubSubClient.h>

#include <ArduinoJson.h>

#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000

WiFiClient client;
MQTTPubSubClient mqtt;
DynamicJsonDocument controlDoc(200);
DynamicJsonDocument biomedicalDoc(200);
PulseOximeter pox;

uint32_t tsLastReport = 0;

const char * ssid = "XVALEE";
const char * pass = "va6333le";

const char * controlTopic = "control";
const char * biomedicalTopic = "biomedical";

// pin definition
const int stressSensor = A0;
const int pump = 2;
const int reliefValve = 3;

int stressSensorValue = 0;
int stress;
bool start;

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

  // initial pump pin output
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);

  // initial relief valve pin output
  pinMode(reliefValve, OUTPUT);
  digitalWrite(reliefValve, LOW);

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

  // subscribe topic and callback which is called when test2 has come
  mqtt.subscribe(controlTopic, [](const String & payload, const size_t size) {
    deserializeJson(controlDoc, payload);
    start = controlDoc["start"];
    stress = controlDoc["stress"];

    // print data to serial
    serializeJson(controlDoc, Serial);
    Serial.println();
  });
}

void loop() {
  pox.update(); // Make sure to call update as fast as possible
  mqtt.update(); // should be called

  // run asynchronously per second
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    // process sensor data to json
    biomedicalDoc["oxygen"] = round(pox.getSpO2());
    biomedicalDoc["heartRate"] = round(pox.getHeartRate());
    String biomedicalData;
    serializeJson(biomedicalDoc, biomedicalData);

    // publish heart rate and oxidation
    mqtt.publish(biomedicalTopic, biomedicalData);

    // process logic when training is start
    if (start) {
      digitalWrite(reliefValve, LOW);

      // calibrate stress sensor value
      // TODO: confirm stressSensor data. stress is between 1 to 10
      stressSensorValue = (analogRead(stressSensor) - 548) * 37.5;

      // open the pump when stress not enough
      if (stressSensorValue != stress) {
        digitalWrite(pump, HIGH);
      } else {
        digitalWrite(pump, LOW);
      }
    } else {
      digitalWrite(reliefValve, HIGH);
    }

    tsLastReport = millis();
  }
}