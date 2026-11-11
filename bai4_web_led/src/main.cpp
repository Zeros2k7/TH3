#include <WiFi.h>
#include <WebServer.h>

const char* wifi_ssid = "ESP32-AP-N11";
const char* wifi_pass = "12345678";

const int LED_PIN = 4;
bool ledState = false;
WebServer server(80);

String page() {
  String s = "<!DOCTYPE html><html><head><meta charset='utf-8'>"
             "<meta name='viewport' content='width=device-width,initial-scale=1'>"
             "<title>ESP32 LED</title></head><body style='font-family:sans-serif;text-align:center'>"
             "<h2>ESP32 LED - Nhom 11 - 25112081</h2>"
             "<p>Trang thai LED: <b>";
  s += ledState ? "BAT" : "TAT";
  s += "</b></p>"
       "<p><a href='/on'><button style='font-size:24px'>BAT</button></a> "
       "<a href='/off'><button style='font-size:24px'>TAT</button></a> "
       "<a href='/toggle'><button style='font-size:24px'>DAO</button></a></p>"
       "</body></html>";
  return s;
}

void setLed(bool on) {
  ledState = on;
  digitalWrite(LED_PIN, on ? HIGH : LOW);
  Serial.println(on ? "LED ON" : "LED OFF");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  setLed(false);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_pass);
  Serial.print("Dang ket noi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Mo trinh duyet: http://");
  Serial.println(WiFi.localIP());

  server.on("/", []() { server.send(200, "text/html", page()); });
  server.on("/on", []() { setLed(true); server.send(200, "text/html", page()); });
  server.on("/off", []() { setLed(false); server.send(200, "text/html", page()); });
  server.on("/toggle", []() {
    setLed(!ledState);
    server.send(200, "text/html", page());
  });
  server.on("/status", []() {
    server.send(200, "application/json", String("{\"led\":\"") + (ledState ? "on" : "off") + "\"}");
  });
  server.onNotFound([]() { server.send(404, "text/plain", "404 Not Found"); });
  server.begin();
}

void loop() {
  server.handleClient();
}
