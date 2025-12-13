# RAIOT_MQTT

**RAIOT_MQTT** is the official Arduino library for the **[RAIOTerm.cc IoT Platform](https://raioterm.cc)**. 
<img width="1919" height="914" alt="image" src="https://github.com/user-attachments/assets/a122963a-ca2b-46a8-9b80-5aeeebdc54b1" />
<img width="1914" height="975" alt="image" src="https://github.com/user-attachments/assets/80404820-abae-4247-a449-bc971b3267dc" />
<img width="1909" height="977" alt="image" src="https://github.com/user-attachments/assets/6d136ea8-756a-4329-b9f3-37ed10fe1c4d" />
<img width="1917" height="977" alt="image" src="https://github.com/user-attachments/assets/8713c151-d495-4681-8366-be8d73a9b24d" />
<img width="1919" height="968" alt="image" src="https://github.com/user-attachments/assets/5cfc9c5c-bdce-460d-b0d8-c05c547aa6da" />
<img width="1915" height="971" alt="image" src="https://github.com/user-attachments/assets/9fe4930d-fc1c-41ee-9728-b39c00df8665" />

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
