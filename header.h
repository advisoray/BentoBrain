#ifndef __BB_HEADER_H__
#define __BB_HEADER_H__

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>

// HTML-specific functions (webserver.cpp)
void handleConfigPage();
void handleSetFanSpeedAPI();
String generateFanSpeedHtmlPage();
String generateConfigHtmlPage();
// Hardware-control functions (hardware.cpp)
void loadSettingsFromNvs();
void saveSettingsToNvs();
void controlFan(float nozzle_temper);
void setupWifi();
// MQTT-specific functions (mqtt.cpp)
void mqttCallback(char *topic, byte *payload, unsigned int length);
void mqttReconnect();
// Arduino functions
void setup();
void loop();

// Object References
extern Preferences preferences;
extern WiFiClientSecure espWifiClient;
extern PubSubClient mqttClient;
extern WebServer webServer;

// Global variables
extern unsigned long lastTimeBelowThreshold;  // Time when temperature dropped below the threshold
extern unsigned long messageCount;            // Counter for the number of messages received
extern unsigned long MQTTconnectCount;        // Counter for the number of reconnections
extern String lastBSSID;                      // Global variable to store the last connected BSSID
extern long lastRSSI;                         // Global variable to store the last RSSI value
extern bool isFanOn;
extern int manualFanSpeed;                    // Used for WebServer logic
extern const int defaultFanSpeed;             // Used for Webserver logic
extern float nozzle_temper;                   // the nozzle temperature
extern float bed_temper;                      // the bed temperature

// User-defined configuration variables
extern char ssid[32];
extern char password[64] ;
extern char mqtt_broker[64];
extern char topic[64];
extern char mqtt_username[32];
extern char mqtt_password[32];
extern int mqtt_port;
extern int fanPin;
extern float tempThreshold;
extern unsigned long fanOffDelay;
extern String authToken;

// Definitions
#define PWM_CHANNEL 0
#define PWM_FREQ 25000    // 25 kHz
#define PWM_RESOLUTION 8  // 8-bit resolution

#endif
