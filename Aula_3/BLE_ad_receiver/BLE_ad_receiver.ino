#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic;

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

bool deviceConnected = false;
bool newDataReceived = false;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        newDataReceived = true;
    }
};

void setup() {
  Serial.begin(115200);

  BLEDevice::init("ESP32_BLE_Server");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  pCharacteristic->setValue("Hello, BLE!");
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
}

void loop() {
  if (deviceConnected) {
    // Check if new data is received
    if (newDataReceived) {
      std::string value = pCharacteristic->getValue();
      Serial.print("Received message: ");
      Serial.println(value.c_str());
      newDataReceived = false; // Reset the flag
    }

    // You can handle writing to the characteristic here if a device is connected
    // For example, periodically updating the characteristic value
    pCharacteristic->setValue("Data from ESP32");
    pCharacteristic->notify();
    delay(1000); // Delay in milliseconds
  }
}
