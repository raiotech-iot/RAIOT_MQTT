/*
  RAIOT_MQTT.h - Library for RAIOT MQTT Protocol.
  Wraps PubSubClient and WiFiClientSecure.
  Handles [FEND][ID]:[VAL][FEND] over MQTT.
*/
#ifndef RAIOT_MQTT_h
#define RAIOT_MQTT_h

#include "Arduino.h"
#include <WiFi.h>              // Use <ESP8266WiFi.h> if using ESP8266
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Protocol Constants
#define RAIOT_FEND 0xC0

// Define the callback function signature for user logic
typedef void (*RaiotCallback)(String id, String value);

class RAIOT_MQTT {
  public:
    RAIOT_MQTT();

    // Setup connection details
    void begin(const char* ssid, const char* pass, 
               const char* mqtt_server, int port, 
               const char* mqtt_user, const char* mqtt_pass, 
               const char* device_id);

    // Set the function that handles incoming messages
    void setCallback(RaiotCallback cb);

    // Must be called in loop() to keep connection alive
    void update();

    // Send data: Formats as [FEND]ID:VAL[FEND] and publishes
    template <typename T>
    void send(String id, T value) {
      if (!_client.connected()) return;

      String valStr = String(value);
      uint8_t buffer[128]; // Buffer for the packet
      int idx = 0;

      buffer[idx++] = RAIOT_FEND; // Start

      // Add ID
      for (unsigned int i = 0; i < id.length(); i++) buffer[idx++] = id[i];
      
      buffer[idx++] = ':'; // Separator

      // Add Value
      for (unsigned int i = 0; i < valStr.length(); i++) buffer[idx++] = valStr[i];

      buffer[idx++] = RAIOT_FEND; // End

      // Publish to cmd/DEVICE_ID/status
      String topic = "cmd/" + String(_deviceId) + "/status";
      _client.publish(topic.c_str(), buffer, idx);
    }

    // Internal usage (public so the global callback can reach it)
    void _handlePacket(char* topic, byte* payload, unsigned int length);

  private:
    void reconnect();
    
    // Credentials
    const char* _ssid;
    const char* _pass;
    const char* _mqttServer;
    int _mqttPort;
    const char* _mqttUser;
    const char* _mqttPass;
    const char* _deviceId;

    // Objects
    WiFiClientSecure _espClient;
    PubSubClient _client;
    
    // User callback pointer
    RaiotCallback _userCallback;
};

// Global Instance
extern RAIOT_MQTT raiotMqtt;

#endif