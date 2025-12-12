/*
  RAIOT_MQTT_Sender.ino
  Demonstrates how to SEND data using the RAIOT_MQTT library.
*/

#include <RAIOT_MQTT.h>

// --- USER CONFIGURATION ---
const char* ssid = "YOUR_WIFI_SSID";
const char* pass = "YOUR_WIFI_PASS";

// MQTT Broker Details
const char* mqtt_server = "mqtt.raioterm.cc";
const int   mqtt_port = 8883; 
const char* client_id = "YOUR_CLIENT_ID"; 
const char* mqtt_user = "YOUR_MQTT_USERNAME";    
const char* mqtt_pass = "YOUR_MQTT_PASSWORD";    

void setup() {
  Serial.begin(115200);

  // Initialize the library
  // This handles WiFi and Secure MQTT connection automatically
  raiotMqtt.begin(ssid, pass, mqtt_server, mqtt_port, mqtt_user, mqtt_pass, client_id);
  
  Serial.println("RAIOT Sender Starting...");
}

void loop() {
  // 1. Keep the connection alive (Essential!)
  raiotMqtt.update();

  // 2. Send data every 5 seconds (Non-blocking delay)
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();

    // Generate fake sensor data
    int tempValue = random(20, 35);
    float humidValue = random(400, 900) / 10.0;

    // Send Temperature -> [FEND]TEMP:25[FEND]
    raiotMqtt.send("TEMP", tempValue);
    
    // Send Humidity -> [FEND]HUM:65.5[FEND]
    raiotMqtt.send("HUM", humidValue);

    Serial.print("Sent update: Temp=");
    Serial.print(tempValue);
    Serial.print(" Hum=");
    Serial.println(humidValue);
  }
}