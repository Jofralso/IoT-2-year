TTGO LoRa boards typically have an ESP32 microcontroller and an onboard LoRa transceiver (like SX1276). Here are two examples for TTGO LoRa boards using the Arduino framework:

### Example 1: Sending Data via LoRa

This example demonstrates how to send data using LoRa on a TTGO LoRa board.

```cpp
#include <SPI.h>
#include <LoRa.h>

#define BAND 915E6  // Adjust according to your LoRa frequency band

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa initialization failed. Check your connections.");
    while (true);
  }
}

void loop() {
  Serial.println("Sending message...");
  LoRa.beginPacket();
  LoRa.print("Hello, LoRa!");
  LoRa.endPacket();
  delay(10000);  // Send every 10 seconds
}
```

### Example 2: Receiving Data via LoRa

This example demonstrates how to receive data using LoRa on a TTGO LoRa board.

```cpp
#include <SPI.h>
#include <LoRa.h>

#define BAND 915E6  // Adjust according to your LoRa frequency band

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa initialization failed. Check your connections.");
    while (true);
  }

  Serial.println("LoRa Initializing OK!");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet: ");

    // Read packet
    while (LoRa.available()) {
      Serial.write(LoRa.read());
    }

    // Print RSSI (Received Signal Strength Indicator)
    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
```

### Instructions:

1. Open the Arduino IDE.
2. Select your TTGO LoRa board from the "Tools" menu.
3. Copy the appropriate code above into the Arduino IDE.
4. Upload the code to your TTGO LoRa board.
5. Open the Serial Monitor to view the transmitted or received data.

Ensure you have the LoRa library installed via the Arduino Library Manager (Sketch > Include Library > Manage Libraries). These examples will work with TTGO LoRa boards, assuming proper connections and configuration for your specific board. Adjust the LoRa frequency band accordingly based on your region and network settings.

----
Certainly! Let's modify the receiver example to display the received sensor data on an OLED display connected to the TTGO LoRa board. We'll use the `Adafruit_SSD1306` library for the OLED display.

### Components Needed:

1. TTGO LoRa board (e.g., TTGO LoRa32)
2. OLED display (e.g., SSD1306)
3. Jumper wires

### Arduino Sketch (Receiver with OLED Display):

```cpp
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BAND 915E6  // Adjust according to your LoRa frequency band

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa initialization failed. Check your connections.");
    while (true);
  }

  if (!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.display();
  delay(2000);  // Pause for 2 seconds
  display.clearDisplay();
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    Serial.println("Received packet:");

    // Read packet and display on OLED
    while (LoRa.available()) {
      char ch = LoRa.read();
      Serial.write(ch);
      display.write(ch);
    }

    display.display();  // Show on OLED
    delay(2000);  // Display for 2 seconds
    display.clearDisplay();
  }
}
```

### Instructions:

1. Install the necessary libraries (LoRa and Adafruit_SSD1306) via the Arduino Library Manager (Sketch > Include Library > Manage Libraries).
2. Wire up the OLED display to the TTGO LoRa board using I2C connections (SDA, SCL).
3. Copy the provided sketch into the Arduino IDE.
4. Upload the code to your TTGO LoRa board.
5. Open the Serial Monitor to view the transmitted sensor data.

The received sensor data will be displayed on the OLED screen for 2 seconds before clearing for the next set of data. Make sure you have the LoRa transmitter sending data so that the receiver can display it on the OLED.

Sure, here's the receiver code for receiving sensor data via LoRa on a TTGO LoRa board and displaying it on an OLED display:

### Arduino Sketch (Receiver with OLED Display):

```cpp
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BAND 915E6  // Adjust according to your LoRa frequency band

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa initialization failed. Check your connections.");
    while (true);
  }

  if (!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.display();
  delay(2000);  // Pause for 2 seconds
  display.clearDisplay();
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    Serial.println("Received packet:");

    // Read packet and display on OLED
    while (LoRa.available()) {
      char ch = LoRa.read();
      Serial.write(ch);
      display.write(ch);
    }

    display.display();  // Show on OLED
    delay(2000);  // Display for 2 seconds
    display.clearDisplay();
  }
}
```

Make sure you have the LoRa transmitter (sender) set up and sending data. This code will receive the data transmitted by the sender and display it on the OLED screen.

Follow these steps:

1. Install the necessary libraries (LoRa and Adafruit_SSD1306) via the Arduino Library Manager (Sketch > Include Library > Manage Libraries).
2. Wire up the OLED display to the TTGO LoRa board using I2C connections (SDA, SCL).
3. Copy the provided sketch into the Arduino IDE.
4. Upload the code to your TTGO LoRa board.
5. Open the Serial Monitor to view the transmitted sensor data.

The received sensor data will be displayed on the OLED screen for 2 seconds before clearing for the next set of data. Adjust the LoRa settings (frequency band, spreading factor, etc.) to match the sender's settings for successful communication.