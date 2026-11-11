#include <WiFi.h>
#include <esp_now.h>

uint8_t receiverMAC[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

typedef struct {
  uint32_t id;
  float value;
  char text[24];
} Packet;

Packet pkt;
uint32_t seq = 0;

void onSent(const wifi_tx_info_t* tx_info, esp_now_send_status_t status) {
  Serial.print("Trang thai gui: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
  (void)tx_info;
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("MAC Transmitter: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("esp_now_init that bai");
    return;
  }
  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, receiverMAC, 6);
  peer.channel = 0;
  peer.encrypt = false;
  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("Them peer that bai");
  }
}

void loop() {
  pkt.id = seq++;
  pkt.value = 20.0 + (random(0, 100) / 10.0);
  strncpy(pkt.text, "Hello N11", sizeof(pkt.text));
  esp_err_t r = esp_now_send(receiverMAC, (uint8_t*)&pkt, sizeof(pkt));
  Serial.printf("Gui id=%lu value=%.1f -> %s\n", (unsigned long)pkt.id, pkt.value,
                r == ESP_OK ? "da dua vao hang doi" : "loi");
  delay(2000);
}
