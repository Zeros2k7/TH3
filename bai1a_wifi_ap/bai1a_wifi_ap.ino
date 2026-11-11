#include <WiFi.h>
const char* AP_SSID = "ESP32-AP-N11";
const char* AP_PASS = "12345678";
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  bool ok = WiFi.softAP(AP_SSID, AP_PASS);
  Serial.println(ok ? "AP started" : "AP failed");
  Serial.print("SSID: ");
  Serial.println(AP_SSID);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}
void loop() {
  Serial.print("So client dang ket noi: ");
  Serial.println(WiFi.softAPgetStationNum());
  delay(3000);
}
