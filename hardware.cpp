#include <header.h>

// Function to load settings from NVS
void loadSettingsFromNvs() {
    preferences.begin("settings", true);  // ReadOnly
    String temp;                          // Temporary String to hold the preference values

    temp = preferences.getString("ssid", ssid);
    strncpy(ssid, temp.c_str(), sizeof(ssid) - 1);
    ssid[sizeof(ssid) - 1] = '\0';  // Ensure null termination

    temp = preferences.getString("password", password);
    strncpy(password, temp.c_str(), sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0';  // Ensure null termination

    temp = preferences.getString("mqtt_broker", mqtt_broker);
    strncpy(mqtt_broker, temp.c_str(), sizeof(mqtt_broker) - 1);
    mqtt_broker[sizeof(mqtt_broker) - 1] = '\0';  // Ensure null termination

    temp = preferences.getString("topic", topic);
    Serial.print("MQTT Topic: ");
    Serial.println(temp);
    strncpy(topic, temp.c_str(), sizeof(topic) - 1);
    topic[sizeof(topic) - 1] = '\0';  // Ensure null termination

    temp = preferences.getString("mqtt_username", mqtt_username);
    strncpy(mqtt_username, temp.c_str(), sizeof(mqtt_username) - 1);
    mqtt_username[sizeof(mqtt_username) - 1] = '\0';  // Ensure null termination

    temp = preferences.getString("mqtt_password", mqtt_password);
    strncpy(mqtt_password, temp.c_str(), sizeof(mqtt_password) - 1);
    mqtt_password[sizeof(mqtt_password) - 1] = '\0';  // Ensure null termination

    fanPin = preferences.getInt("fanPin", fanPin);
    tempThreshold = preferences.getFloat("tempThreshold", tempThreshold);
    fanOffDelay = preferences.getULong("fanOffDelay", fanOffDelay);
    authToken = preferences.getString("authToken", authToken);
    preferences.end();
}

// Function to save settings to NVS
void saveSettingsToNvs() {
    preferences.begin("settings", false);  // ReadWrite
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.putString("mqtt_broker", mqtt_broker);
    preferences.putString("topic", topic);
    preferences.putString("mqtt_username", mqtt_username);
    preferences.putString("mqtt_password", mqtt_password);
    preferences.putInt("mqtt_port", mqtt_port);
    preferences.putInt("fanPin", fanPin);
    preferences.putFloat("tempThreshold", tempThreshold);
    preferences.putULong("fanOffDelay", fanOffDelay);
    preferences.putString("authToken", authToken);
    preferences.end();
}

void setupWifi() {
    delay(10);
    // Connect to WiFi
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // OTA event handlers setup
    ArduinoOTA.onStart([]() {
        // Code to run on start of OTA update
        Serial.println("OTA Update Start");
    });
    ArduinoOTA.onEnd([]() {
        // Code to run on end of OTA update
        Serial.println("\nOTA Update End");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        // Code to run during OTA update progress
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        // Code to run on OTA update error
        Serial.printf("OTA Error [code %u]: ", error);
        if (error == OTA_AUTH_ERROR) {Serial.println("Auth Failed");}
        else if (error == OTA_BEGIN_ERROR) {Serial.println("Begin Failed");}
        else if (error == OTA_CONNECT_ERROR) {Serial.println("Connect Failed");}
        else if (error == OTA_RECEIVE_ERROR) {Serial.println("Receive Failed");}
        else if (error == OTA_END_ERROR) {Serial.println("End Failed"); }
    });

    // Initialize OTA
    ArduinoOTA.begin();

    Serial.println("");
    Serial.print("Connected to SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("Connection Quality (RSSI)): ");
    Serial.println(WiFi.RSSI());
    Serial.println("");
    Serial.print("WiFi connected - IP address: ");
    Serial.println(WiFi.localIP());
}

void controlFan(float nozzle_temper) {
    if (manualFanSpeed >= 0 && manualFanSpeed <= 100) {
        // Manual control: Convert 1-100 range to 0-255 PWM value
        ledcWrite(PWM_CHANNEL, map(manualFanSpeed, 0, 100, 0, 255));
        isFanOn = true;
    } else {
        // Automatic control based on temperature
        if (nozzle_temper > tempThreshold) {
            ledcWrite(PWM_CHANNEL, 255);
            isFanOn = true;
        } else if (nozzle_temper < tempThreshold && isFanOn) {
            lastTimeBelowThreshold = millis();
            isFanOn = false;
        }

        // If fan is off and manualFanSpeed is -1 (automatic mode), turn off the fan immediately
        if (!isFanOn) {
            ledcWrite(PWM_CHANNEL, 0);
        }
    }
}
