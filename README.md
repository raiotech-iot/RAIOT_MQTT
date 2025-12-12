# RAIOT_MQTT

**RAIOT_MQTT** is the official Arduino library for the **[RAIOTerm.cc IoT Platform](https://raioterm.cc)**. 

It wraps `PubSubClient` and `WiFiClientSecure` to simplify sending and receiving data using the **RAIOT Protocol** (`[FEND][ID]:[VAL][FEND]`). It automates connection management (WiFi & MQTT auto-reconnect), packet framing, and data parsing, allowing you to focus on your sensor logic.

## ✨ Features
* **Designed for RAIOTerm.cc:** Seamless integration with the RAIOTerm dashboard.
* **Protocol Automation:** Automatically wraps data with `[FEND]` markers (`0xC0`).
* **Type-Safe Sending:** Supports `int`, `float`, `uint16_t`, `String`, etc. automatically.
* **Auto-Reconnect:** Built-in logic to handle WiFi and MQTT drops.
* **Secure:** Uses `WiFiClientSecure` by default.

## 📦 Dependencies
This library depends on:
* **PubSubClient** by Nick O'Leary.
* *The Arduino Library Manager should install this automatically.*

## 🛠 Installation
1.  Open Arduino IDE.
2.  Go to **Sketch > Include Library > Manage Libraries**.
3.  Search for **RAIOT_MQTT**.
4.  Click **Install**.

## 🚀 Usage

### Sender Example
```cpp

#include <RAIOT_MQTT.h>

void myCallback(String id, String value) {
  if (id == "LED" && value == "ON") {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void setup() {
  raiotMqtt.begin("SSID", "PASS", "mqtt.raioterm.cc", 8883, "user", "pass", "clientID");
  raiotMqtt.setCallback(myCallback);
}

void loop() {
  raiotMqtt.update();
}

void setup() {
  // Connect to the RAIOTerm.cc Broker
  raiotMqtt.begin("SSID", "PASS", "mqtt.raioterm.cc", 8883, "user", "pass", "clientID");
}

void loop() {
  raiotMqtt.update(); // Keeps connection alive

  // Send integer data
  uint16_t co2 = 450;
  raiotMqtt.send("CO2", co2); // Sends: [FEND]CO2:450[FEND]
  
  delay(1000);
}
