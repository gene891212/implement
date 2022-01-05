#include <WiFi.h>
#include <MQTTPubSubClient.h>

const char* ssid = "XVALEE";
const char* pass = "va6333le";
char buf[40];
WiFiClient client;
MQTTPubSubClient mqtt;

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
  mqtt.subscribe("test2", [](const String & payload, const size_t size) {
    Serial.println(payload);
  });
}

void loop() {
  mqtt.update();  // should be called

  // publish message
  //    static uint32_t prev_ms = millis();
  //    if (millis() > prev_ms + 1000) {
  //        prev_ms = millis();
  //        mqtt.publish("/hello", "world");
  //    }
  sprintf(buf, "{\"oxygen\": \"%d\"}", random(30, 100));
  mqtt.publish("test2", buf);
  delay(1000);
}
