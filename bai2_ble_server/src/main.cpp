#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer* pServer = nullptr;
BLECharacteristic* pChar = nullptr;
bool deviceConnected = false;
String deviceMessage = "MSSV: 25112081";

class ServerCB : public BLEServerCallbacks {
  void onConnect(BLEServer*) override {
    deviceConnected = true;
    Serial.println("BLE client connected");
  }

  void onDisconnect(BLEServer*) override {
    deviceConnected = false;
    Serial.println("BLE client disconnected, advertising again");
    BLEDevice::startAdvertising();
  }
};

class CharCB : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* characteristic) override {
    String value = characteristic->getValue();
    if (!value.isEmpty()) {
      deviceMessage = value;
      characteristic->setValue(deviceMessage.c_str());
      Serial.print("Client wrote: ");
      Serial.println(deviceMessage);
    }
  }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32-BLE-N11-25112081");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCB());

  BLEService* svc = pServer->createService(SERVICE_UUID);
  pChar = svc->createCharacteristic(
      CHAR_UUID,
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_NOTIFY);

  pChar->addDescriptor(new BLE2902());
  pChar->setCallbacks(new CharCB());
  pChar->setValue(deviceMessage.c_str());
  svc->start();

  BLEAdvertising* adv = BLEDevice::getAdvertising();
  adv->addServiceUUID(SERVICE_UUID);
  adv->setScanResponse(true);
  BLEDevice::startAdvertising();
  Serial.println("BLE advertising: ESP32-BLE-N11-25112081");
}

void loop() {
  if (deviceConnected) {
    pChar->setValue(deviceMessage.c_str());
    pChar->notify();
    Serial.println("Notify: " + deviceMessage);
  }
  delay(2000);
}
