#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <AsyncMqttClient.h>
#include <json/json.h>

const char* ssid = "EmbeddedWorkshop";
const char* password = "stc6workshop";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, D8, NEO_RGB + NEO_KHZ800);

WiFiClient net;
AsyncMqttClient mqtt_client;
#define MQTT_HOST IPAddress(10, 42, 0, 1)
#define MQTT_PORT 1883

unsigned long lastMillis = 0;

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  uint16_t packetIdSub = mqtt_client.subscribe("all/color", 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  String topic_str(topic);
  if(topic_str.equals("all/color")) {
    JsonDocument json_doc;
    deserializeJson(json_doc, payload);

    int led = json_doc["led"];
    int red = json_doc["r"];
    int green = json_doc["g"];
    int blue = json_doc["b"];

    strip.setPixelColor(led, strip.Color(red, green, blue));
    strip.show();
  }
}

void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.setBrightness(255);
  strip.show();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connection established!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // MQTT
  mqtt_client.onConnect(onMqttConnect);
  mqtt_client.onMessage(onMqttMessage);
  mqtt_client.setServer(MQTT_HOST, MQTT_PORT);

  mqtt_client.connect();
}


void loop() {
}
