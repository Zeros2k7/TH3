#include <WiFi.h>
#include <PubSubClient.h>

const char* wifi_ssid = "ESP32-AP-N11";
const char* wifi_pass = "12345678";

const char* mqtt_host = "broker.hivemq.com";
const uint16_t mqtt_port = 1883;
const char* TOPIC_PUB = "bcse/th3/nhom-11";
const char* TOPIC_SUB = "bcse/th3/nhom-11/led";

const int LED_PIN = 2;
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
unsigned long lastPub = 0;

void onMessage(char* topic, byte* payload, unsigned int len) {
  String msg;
  for (unsigned int i = 0; i < len; i++) {
    msg += (char)payload[i];
  }

  Serial.printf("Nhan [%s]: %s\n", topic, msg.c_str());
  msg.trim();
  if (msg == "1") {
    digitalWrite(LED_PIN, HIGH);
  } else if (msg == "0") {
    digitalWrite(LED_PIN, LOW);
  }
}

void connectMqtt() {
  while (!mqtt.connected()) {
    String cid = "esp32-bcse-n11-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    Serial.print("Ket noi MQTT... ");
    if (mqtt.connect(cid.c_str())) {
      Serial.println("OK");
      mqtt.subscribe(TOPIC_SUB);
      Serial.printf("Da subscribe %s\n", TOPIC_SUB);
    } else {
      Serial.printf("loi rc=%d, thu lai sau 3s\n", mqtt.state());
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  mqtt.setServer(mqtt_host, mqtt_port);
  mqtt.setCallback(onMessage);
}

void loop() {
  if (!mqtt.connected()) {
    connectMqtt();
  }
  mqtt.loop();

  if (millis() - lastPub > 5000) {
    lastPub = millis();
    static uint32_t counter = 0;
    char buf[32];
    snprintf(buf, sizeof(buf), "%lu", (unsigned long)counter++);
    mqtt.publish(TOPIC_PUB, buf);
    Serial.printf("Publish [%s]: %s\n", TOPIC_PUB, buf);
  }
}
