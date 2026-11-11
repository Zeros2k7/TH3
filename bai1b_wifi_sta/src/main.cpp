#include <WiFi.h>

const char* wifi_ssid = "ESP32-AP-N11";
const char* wifi_pass = "12345678";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_pass);

  Serial.print("Dang ket noi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Da ket noi. IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI (dBm): ");
  Serial.println(WiFi.RSSI());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Mat ket noi, thu lai...");
    WiFi.reconnect();
  } else {
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
  }
  delay(5000);
}
