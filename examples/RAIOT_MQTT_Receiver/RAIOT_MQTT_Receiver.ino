/*
  RAIOT_MQTT_Receiver.ino
  Demonstrates how to RECEIVE data using the RAIOT_MQTT library.
*/

#include <RAIOT_MQTT.h>

// --- USER CONFIGURATION ---
const char* ssid = "YOUR_WIFI_SSID";
const char* pass = "YOUR_WIFI_PASS";

const char* mqtt_server = "mqtt.raioterm.cc";
const int   mqtt_port = 8883; 
const char* client_id = "YOUR_MQTT_CLIENT_ID"; 
const char* mqtt_user = "YOUR_MQTT_USERNAME";    
const char* mqtt_pass = "YOUR_MQTT_PASSWORD"; 

// --- CALLBACK FUNCTION ---
// This function runs automatically whenever a valid packet arrives.
// Protocol: [FEND]ID:VALUE[FEND]
void onDataReceived(String id, String value) {
  Serial.print("Command: ");
  Serial.print(id);
  Serial.print(" -> Value: ");
  Serial.println(value);

  // Logic to control hardware
  if (id == "LED") {
    if (value == "ON" || value == "1") {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println(">> LED turned ON");
    } 
    else if (value == "OFF" || value == "0") {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println(">> LED turned OFF");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // 1. Initialize connection
  raiotMqtt.begin(ssid, pass, mqtt_server, mqtt_port, mqtt_user, mqtt_pass, client_id);

  // 2. REGISTER THE CALLBACK
  // This tells the library: "When you get data, call the 'onDataReceived' function"
  raiotMqtt.setCallback(onDataReceived);
  
  Serial.println("RAIOT Receiver Listening...");
}

void loop() {
  // Keep connection alive and listen for incoming packets
  raiotMqtt.update();
}