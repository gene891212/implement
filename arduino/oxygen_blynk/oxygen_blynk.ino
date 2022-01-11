#include <Wire.h>

#include "MAX30100_PulseOximeter.h"

#define BLYNK_PRINT Serial#include <Blynk.h>

#include <ESP8266WiFi.h>

#include <BlynkSimpleEsp8266.h>

#define REPORTING_PERIOD_MS 1000

char auth[] = "<blynk-auth>";
char ssid[] = "AndroidAP";
char pass[] = "123456789";

// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0
PulseOximeter pox;

float BPM, SpO2;
uint32_t tsLastReport = 0;
int sensor = A0;
int sensorRead = 0;
int i = 0;
void setup() {

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  Serial.print("Initializing Pulse Oximeter..");
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop() {

  sensorRead = analogRead(sensor);
  pox.update();
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  Serial.print("Heart rate:");
  Serial.print(BPM);
  Serial.print(" bpm / SpO2:");
  Serial.print(SpO2);
  Serial.println(" %");
  Serial.println((sensorRead));
  Blynk.run();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Blynk.virtualWrite(V7, BPM);
    Blynk.virtualWrite(V8, SpO2);
    Blynk.virtualWrite(V0, (sensorRead - 548) * 37.5);
    tsLastReport = millis();
    delay(1000);
  }

}