#include <header.h>

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    messageCount++;  // Increment the message count

    Serial.print("Message ");
    Serial.print(messageCount);
    Serial.print(" arrived on MQTT Connection #");
    Serial.println(MQTTconnectCount);

    // Reduce the size of the JSON document
    DynamicJsonDocument doc(1024);  // Adjust this size

    DeserializationError error = deserializeJson(doc, payload, length);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }
    // Extract the device temperatures
    nozzle_temper = doc["print"]["nozzle_temper"];
}

void mqttReconnect() {
    if (mqttClient.connected()) {
        return;
    }
    MQTTconnectCount++;  // Increment the reconnect counter
    messageCount = 0;    // Reset the message count on reconnect

    Serial.print("Connection attempt #");
    Serial.println(MQTTconnectCount);

    // Loop until we're reconnected
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");

        // Attempt to connect
        if (mqttClient.connect("BentoBrain", mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT Broker!");
            // Subscribe to the topic
            mqttClient.subscribe(topic);
            Serial.println("Subscribed to topic: " + String(topic));
        } else {
            Serial.print("Failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" trying again in 5 seconds");

            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}
