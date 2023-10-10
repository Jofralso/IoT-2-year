Simple example where we'll use an Arduino with an ESP32 to send temperature data (simulated using a potentiometer) over BLE to another Arduino with an ESP32 acting as a BLE client. The client will read the temperature data and display it on a serial monitor.

### Components Needed:

1. 2 x Arduino boards with ESP32 (e.g., ESP32 Dev Module)
2. 2 x Potentiometers
3. Jumper wires

### Circuit Connections:

- **Arduino 1 (BLE Server):**

  - Connect the potentiometer to analog pin A0.
  - Connect the potentiometer's other legs to 3.3V and GND.

- **Arduino 2 (BLE Client):**

  - No additional hardware required for this example.

### Arduino Sketch (BLE Server - Temperature Sender):

```cpp
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer;
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char *CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

void setup() {
  Serial.begin(115200);

  BLEDevice::init("ESP32_BLE_Server");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ
  );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  pServer->getAdvertising()->start();
}

void loop() {
  pServer->loop();
  
  if (deviceConnected) {
    // Simulate a temperature reading (potentiometer value)
    int temperatureValue = analogRead(A0);
    pCharacteristic->setValue(temperatureValue);
  }
}

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};
```

### Arduino Sketch (BLE Client - Temperature Receiver):

```cpp
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>

BLEScan* pBLEScan;

void setup() {
  Serial.begin(115200);
  
  BLEDevice::init("ESP32_BLE_Client");
  pBLEScan = BLEDevice::getScan(); // Get a new scan object

  pBLEScan->setActiveScan(true); // Active scan uses more power, but gets results faster
}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(5, false);
  Serial.println("Scan done!");

  for (int i = 0; i < foundDevices.getCount(); i++) {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    Serial.print("Found BLE Device: ");
    Serial.println(device.toString().c_str());

    if (device.haveServiceUUID()) {
      if (device.getServiceUUID().equals(BLEUUID(SERVICE_UUID))) {
        Serial.println("Found our service");

        BLEClient* pClient = BLEDevice::createClient();
        pClient->connect(&device);

        if (pClient->isConnected()) {
          Serial.println("Connected to server");
          delay(1000);

          BLERemoteCharacteristic* pRemoteCharacteristic = pClient->getService(BLEUUID(SERVICE_UUID))->getCharacteristic(BLEUUID(CHARACTERISTIC_UUID));
          if (pRemoteCharacteristic->canRead()) {
            std::string value = pRemoteCharacteristic->readValue();
            Serial.print("Temperature Value: ");
            Serial.println(*(uint32_t*)value.data());
          }

          pClient->disconnect();
        }
        delete pClient;
      }
    }
  }
}
```

### Instructions:

1. Upload the "BLE Server" sketch to Arduino 1 (ESP32 acting as BLE Server).
2. Open the Serial Monitor for Arduino 1 and note the device's MAC address (you'll need this for the client).
3. Upload the "BLE Client" sketch to Arduino 2 (ESP32 acting as BLE Client).
4. Update the MAC address in the client sketch with the MAC address from Arduino 1.
5. Open the Serial Monitor for Arduino 2.
6. Turn the potentiometer on Arduino 1 to simulate temperature changes.
7. You should see the temperature value (analog reading from the potentiometer) displayed on the Serial Monitor for Arduino 2.

This example demonstrates how to send simulated temperature data from one Arduino to another over BLE using ESP32. Modify the code and hardware connections to fit your specific use case with a gas sensor.

----
Simple example where an Arduino (ESP32) acts as a BLE server to control a simulated "smart" light bulb. We'll simulate a light bulb using an LED, and you'll be able to turn it on and off from a companion Android app acting as the BLE client.

### Components Needed:

1. Arduino board with ESP32 (e.g., ESP32 Dev Module)
2. LED
3. Resistor (220-330 Ohms)
4. Android smartphone

### Circuit Connections:

- Connect the LED's anode (longer leg) to pin 13 through a resistor.
- Connect the LED's cathode (shorter leg) to ground.

### Arduino Sketch (BLE Server - Smart Light Bulb):

```cpp
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer;
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
bool isLightOn = false;

const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char *CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

void setup() {
  Serial.begin(115200);
  
  pinMode(13, OUTPUT); // LED

  BLEDevice::init("ESP32_BLE_Server");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  pServer->getAdvertising()->start();
}

void loop() {
  pServer->loop();
  
  if (deviceConnected) {
    // Update the LED based on the light state
    digitalWrite(13, isLightOn ? HIGH : LOW);
  }
}

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setLightState(bool newState) {
  isLightOn = newState;
}
```

### Android App: BLE Client for Smart Light Bulb

Create an Android app using Android Studio to serve as the BLE client to control the "smart" light bulb. The app will have a button to toggle the light on and off.

The Android app should scan for BLE devices, connect to the ESP32 BLE server, and then send commands to turn the light on and off using the BLE characteristic.

### Usage Steps:

1. Upload the "BLE Server" sketch to the Arduino (ESP32).
2. Run the Android app on your smartphone.
3. Connect to the ESP32 BLE server from the app.
4. Use the app to control the light bulb: turn it on and off.

In this example, you've created a "smart" light bulb using an Arduino and simulated control over BLE. The Android app will connect to the ESP32 BLE server and send commands to turn the LED on and off. You can extend this example to include more features, like changing brightness or color, based on your specific requirements for a smart home setup.