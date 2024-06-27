#include <header.h>

/**
 * Configuration variables for BentoBrain ESP32
 **/

char ssid[32] = "network_id"; // WiFi network ID
char password[64] = "network_password"; // WiFi network password
char mqtt_broker[64] = "192.168.1.66"; // IP Address of your Bambu Lab 3d printer (X1 / P1)
char topic[64] = "device/+/report"; // MQTT topic we will subscribe to for updates
char mqtt_username[32] = "bblp"; // The default mqtt user name for Bambu Lab printers
char mqtt_password[32] = "<password>"; //3d Printer password obtained from LCD screen LAN Management
int mqtt_port = 8883; // Port on the 3d printer on which the MQTT broker runs
int fanPin = 6; // The hardware pin that controls the fant
float tempThreshold = 180.0; // The temperature threshold of the nozzle above which to turn on / off the fan
unsigned long fanOffDelay = 300000; // how many milliseconds after we go below the temperature threshold to turn off the fan
String authToken = "-1"; // authorization token for the internal ESP32 BentoBrain API
