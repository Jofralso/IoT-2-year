#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>

BLEServer *pServer = NULL;
BLEAdvertising *pAdvertising = NULL;

#define SERVICE_UUID "4c94e428-2e57-4106-b708-8e02e188e426"
#define CHARACTERISTIC_UUID "40ce28ef-8b59-4df7-8dd8-8eb16d53d93b"

BLECharacteristic *pCharacteristic;


void setup() {
  Serial.begin(115200);

//Criar BLE Device
BLEDevice::init("ESP32_BLE_Server");

pServer = BLEDevice::createServer();

BLEService *pService = pServer->createService(SERVICE_UUID);

pCharacteristic = pService->createCharacteristic(
  CHARACTERISTIC_UUID,  
  BLECharacteristic::PROPERTY_READ |  
  BLECharacteristic::PROPERTY_WRITE
);
pCharacteristic->setValue("Hello, BLE!");

pService->start();

pAdvertising = pServer->getAdvertising();
pAdvertising->start();
}

void loop() {
  // put your main code here, to run repeatedly:

}
