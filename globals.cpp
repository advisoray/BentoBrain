#include <header.h>

// Global variables
unsigned long lastTimeBelowThreshold = 0;  // Time when temperature dropped below the threshold
unsigned long messageCount = 0;            // Counter for the number of messages received
unsigned long MQTTconnectCount = 0;        // Counter for the number of reconnections
String lastBSSID = "";                     // Global variable to store the last connected BSSID
long lastRSSI = 0;                         // Global variable to store the last RSSI value
bool isFanOn = false;
int manualFanSpeed = -1;                   // Used for WebServer logic
const int defaultFanSpeed = -1;            // Used for Webserver logic
float nozzle_temper = 0;                   // the nozzle temperature
float bed_temper = 0;                      // the bed temperature

