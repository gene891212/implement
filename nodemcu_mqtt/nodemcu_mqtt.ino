#include <WiFi.h>

#include <MQTTPubSubClient.h>

#include <ArduinoJson.h>

WiFiClient client;
MQTTPubSubClient mqtt;
DynamicJsonDocument doc(200);

const char * ssid = "XVALEE";
const char * pass = "va6333le";
const char * controlTopic = "control";
const char * biomedicalTopic = "biomedical";
int stress;
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
  mqtt.subscribe(controlTopic, [](const String & payload, const size_t size) {
    deserializeJson(doc, payload);
    pump = doc["pump"];
    stress = doc["stress"];
    // Serial.println(doc["pump"].as<bool>());
    serializeJson(doc, Serial);
  });
}

void loop() {
  mqtt.update(); // should be called
  // publish message
  //    static uint32_t prev_ms = millis();
  //    if (millis() > prev_ms + 1000) {
  //        prev_ms = millis();
  //        mqtt.publish("/hello", "world");
  //    }
  sprintf(buf, "{\"oxygen\": \"%d\"}", random(30, 100));
  mqtt.publish(biomedicalTopic, buf);
  delay(1000);
}
