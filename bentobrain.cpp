#include <header.h>

Preferences preferences;
WiFiClientSecure espWifiClient;
PubSubClient mqttClient(espWifiClient);
WebServer webServer(80);

/**
 * Function that is run when the esp32 first starts
 */
void setup() {

    Serial.begin(115200);
    setupWifi();
    mqttClient.setServer(mqtt_broker, mqtt_port);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setBufferSize(12000);

    // Load settings from NVS
    loadSettingsFromNvs();

    // Set the keep-alive interval (e.g., 60 seconds)
    mqttClient.setKeepAlive(60);

    // Configure WiFiClientSecure to accept self-signed certificates
    espWifiClient.setInsecure();

    // Configure PWM Details
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(fanPin, PWM_CHANNEL);

    // Define server routes
    webServer.on("/", HTTP_GET, []() {
        webServer.send(200, "text/html", generateFanSpeedHtmlPage());
    });

    webServer.on("/set", HTTP_POST, []() {
    if (webServer.hasArg("value")) {
      manualFanSpeed = webServer.arg("value").toInt();
    }
    webServer.sendHeader("Location", "/", true);
    webServer.send(302, "text/plain", ""); });

    webServer.on("/reset", HTTP_GET, []() {
    manualFanSpeed = defaultFanSpeed;
    webServer.sendHeader("Location", "/", true);
    webServer.send(302, "text/plain", ""); });
    webServer.on("/config", handleConfigPage);
    webServer.on("/api/setFanSpeed", handleSetFanSpeedAPI);

    // Start the server
    webServer.begin();
}

/**
 * Function that runs continuously to check for any new messages and handle them appropriately
 */
void loop() {
    webServer.handleClient();

    // Handle OTA
    ArduinoOTA.handle();
    // Handle manual fan speed changes instantly
    if (manualFanSpeed != -1 || (manualFanSpeed == -1 && !isFanOn)) {
        controlFan(nozzle_temper);  // You need to ensure nozzle_temper is available here, or fetch it before this line.
    }
    if (!mqttClient.connected()) {
        mqttReconnect();
    }
    mqttClient.loop();

    // Turn off the fan after the specified delay duration if in automatic mode
    if (manualFanSpeed == -1 && isFanOn && millis() - lastTimeBelowThreshold > fanOffDelay) {
        ledcWrite(PWM_CHANNEL, 0);
        isFanOn = false;
    }
}
