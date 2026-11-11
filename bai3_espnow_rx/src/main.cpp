#include <WiFi.h>
#include <esp_now.h>

typedef struct {
  uint32_t id;
  float value;
  char text[24];
} Packet;

void onRecv(const esp_now_recv_info_t* info, const uint8_t* data, int len) {
  Packet p;
  memcpy(&p, data, sizeof(p));
  Serial.printf("Nhan %d byte tu %02X:%02X:%02X:%02X:%02X:%02X | id=%lu value=%.1f text=%s\n",
                len,
                info->src_addr[0], info->src_addr[1], info->src_addr[2],
                info->src_addr[3], info->src_addr[4], info->src_addr[5],
                (unsigned long)p.id, p.value, p.text);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("MAC Receiver: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("esp_now_init that bai");
    return;
  }
  esp_now_register_recv_cb(onRecv);
  Serial.println("San sang nhan du lieu ESP-NOW");
}

void loop() {
  delay(1000);
}
