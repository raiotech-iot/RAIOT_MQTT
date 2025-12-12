#include "RAIOT_MQTT.h"

// -----------------------------------------------------------
// Global Trampoline Callback
// PubSubClient requires a static function, so we route it here
// -----------------------------------------------------------
void _internalMqttCallback(char* topic, byte* payload, unsigned int length) {
  raiotMqtt._handlePacket(topic, payload, length);
}

// Constructor
RAIOT_MQTT::RAIOT_MQTT() : _client(_espClient) {
  _userCallback = NULL;
}

void RAIOT_MQTT::begin(const char* ssid, const char* pass, 
                       const char* mqtt_server, int port, 
                       const char* mqtt_user, const char* mqtt_pass, 
                       const char* device_id) {
  _ssid = ssid;
  _pass = pass;
  _mqttServer = mqtt_server;
  _mqttPort = port;
  _mqttUser = mqtt_user;
  _mqttPass = mqtt_pass;
  _deviceId = device_id;

  // 1. Setup WiFi
  WiFi.begin(_ssid, _pass);
  // (Blocking wait usually avoided in libs, but necessary for init)
  // We allow the user to handle the "waiting" visual in their setup if they want, 
  // but here we ensure the client is configured.
  
  // 2. Setup Secure Client
  _espClient.setInsecure(); // Skip cert validation (as per your example)

  // 3. Setup MQTT
  _client.setServer(_mqttServer, _mqttPort);
  _client.setCallback(_internalMqttCallback);
}

void RAIOT_MQTT::setCallback(RaiotCallback cb) {
  _userCallback = cb;
}

void RAIOT_MQTT::update() {
  // Ensure WiFi is connected
  if (WiFi.status() != WL_CONNECTED) {
    // Simple reconnect attempt (blocking)
    // In a production lib, non-blocking is better, but this matches your example logic
    WiFi.begin(_ssid, _pass); 
    return; 
  }

  // Ensure MQTT is connected
  if (!_client.connected()) {
    reconnect();
  }
  
  _client.loop();
}

void RAIOT_MQTT::reconnect() {
  // Try to connect
  if (_client.connect(_deviceId, _mqttUser, _mqttPass)) {
    // Subscribe to cmd/DEVICE_ID/command
    String subTopic = "cmd/" + String(_deviceId) + "/command";
    _client.subscribe(subTopic.c_str());
  }
}

// -----------------------------------------------------------
// Parsing Logic (The core "Read Live Data" part)
// -----------------------------------------------------------
void RAIOT_MQTT::_handlePacket(char* topic, byte* payload, unsigned int length) {
  // 1. Safety Check (FEND bytes)
  if (length < 3 || payload[0] != RAIOT_FEND || payload[length - 1] != RAIOT_FEND) {
    return; // Invalid packet
  }

  // 2. Extract the string between FENDs
  String cleanData = "";
  for (unsigned int i = 1; i < length - 1; i++) {
    cleanData += (char)payload[i];
  }

  // 3. Split by ':' to get ID and Value
  int splitIndex = cleanData.indexOf(':');
  if (splitIndex > 0) {
    String cmdId = cleanData.substring(0, splitIndex);
    String val = cleanData.substring(splitIndex + 1);

    // 4. Trigger User Callback
    if (_userCallback != NULL) {
      _userCallback(cmdId, val);
    }
  }
}

// Instantiate Global Object
RAIOT_MQTT raiotMqtt;