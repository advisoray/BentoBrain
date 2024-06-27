#include <header.h>

/**
 *  HTML pages for web server (settings config)
 */

// Function to generate HTML content based on manualFanSpeed
String generateFanSpeedHtmlPage() {
    String htmlContent =
        "<!DOCTYPE html>"
        "<html><head>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "<style>"
        "body { background-color: #FFFFFF; font-family: Arial, sans-serif; margin: 0; padding: 40px; font-size: 20px; color: rgb(255,90,44); }"
        "h1 { font-size: 2em; }"
        "input[type=range] { -webkit-appearance: none; margin: 18px 0; width: 100%; height: 25px; background: rgb(255,90,44); outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s; }"
        "input[type=range]::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #000000; cursor: pointer; }"
        "input[type=range]::-moz-range-thumb { width: 35px; height: 35px; background: #000000; cursor: pointer; }"
        "button[type=button] { background-color: rgb(255,90,44); color: #FFFFFF; font-size: 1em; padding: 10px; border: none; cursor: pointer; }"
        "input[type=submit] { display: none; }"
        "</style>"
        "</head>"
        "<body>"
        "<h1>BentoBrain<br/>Fan Control</h1>"
        "<p>Current Value: " +
        (manualFanSpeed == -1 ? "Automatic" : (String(manualFanSpeed) + "%")) +
        "</p>"
        "<form action=\"/set\" method=\"POST\">"
        "<input type=\"range\" id=\"slider\" class=\"slider\" name=\"value\" min=\"0\" max=\"100\" step=\"10\" value=\"" +
        (manualFanSpeed == -1 ? "0" : String(manualFanSpeed)) +
        "\" oninput=\"updateValue(value)\" onchange=\"sendValue()\" style=\"width:300px;\"><br>"
        "<input type=\"submit\" value=\"Set Fan Percent\">"
        "<button type=\"button\" style=\"width:300px;\" onclick=\"setAutomatic()\">Set to Automatic</button>"
        "</form>"
        "<a href=\"/config\" style=\"display:block; width:300px; text-align:center; background-color: rgb(255,90,44); color: #FFFFFF; font-size: 1em; padding: 10px; border: none; cursor: pointer; text-decoration: none; margin-top: 10px;\">Config</a>"

        "<script>"
        "function updateValue(val) {"
        "  document.querySelector('p').textContent = 'Current Value: ' + val + '%';"
        "}"
        "function sendValue() {"
        "  var val = document.getElementById('slider').value;"
        "  var xhr = new XMLHttpRequest();"
        "  xhr.open('POST', '/set', true);"
        "  xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');"
        "  xhr.send('value=' + val);"
        "}"
        "function setAutomatic() {"
        "  document.querySelector('p').textContent = 'Current Value: Automatic';"
        "  var xhr = new XMLHttpRequest();"
        "  xhr.open('POST', '/set', true);"
        "  xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');"
        "  xhr.send('value=-1');"
        "  document.getElementById('slider').value = -1;"
        "}"
        "</script>"
        "</body>"
        "</html>";

    return htmlContent;
}
// String that generates the Config Page
String generateConfigHtmlPage() {
    String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>ESP32 Config Page</title></head><body>";
    html += "<h2>ESP32 Configuration</h2>";
    html += "<form method='post' action='/config'>";

    // Network Credentials
    html += "<h3>Network Credentials</h3>";
    html += "SSID:<br><input type='text' name='ssid' value='" + String(ssid) + "'><br>";
    html += "Password:<br><input type='text' name='password' value='" + String(password) + "'><br>";

    // Printer Settings
    html += "<h3>Printer Settings</h3>";
    html += "MQTT Broker:<br><input type='text' name='mqtt_broker' value='" + String(mqtt_broker) + "'><br>";
    html += "Topic:<br><input type='text' name='topic' value='" + String(topic) + "'><br>";
    html += "MQTT Username:<br><input type='text' name='mqtt_username' value='" + String(mqtt_username) + "'><br>";
    html += "MQTT Password:<br><input type='text' name='mqtt_password' value='" + String(mqtt_password) + "'><br>";
    html += "MQTT Port:<br><input type='number' name='mqtt_port' value='" + String(mqtt_port) + "'><br>";

    // Other Settings
    html += "<h3>Other Settings</h3>";
    html += "Fan Pin:<br><input type='number' name='fanPin' value='" + String(fanPin) + "'><br>";
    html += "Temperature Threshold:<br><input type='number' name='tempThreshold' value='" + String(tempThreshold) + "' step='0.1'><br>";
    html += "Fan Off Delay:<br><input type='number' name='fanOffDelay' value='" + String(fanOffDelay) + "'><br>";

    // API Key
    html += "<h3>API Key</h3>";
    html += "Auth Token:<br><input type='text' name='authToken' value='" + authToken + "'><br>";
    html += "<input type='submit' name='action' value='Save'>";
    html += "<input type='submit' name='action' value='Save and Restart'>";
    html += "</form></body></html>";
    return html;
}

void handleConfigPage() {
    if (webServer.method() == HTTP_GET) {
        webServer.send(200, "text/html", generateConfigHtmlPage());
    } else if (webServer.method() == HTTP_POST) {
        // Parse and save new settings
        strncpy(ssid, webServer.arg("ssid").c_str(), sizeof(ssid));
        strncpy(password, webServer.arg("password").c_str(), sizeof(password));
        strncpy(mqtt_broker, webServer.arg("mqtt_broker").c_str(), sizeof(mqtt_broker));
        strncpy(topic, webServer.arg("topic").c_str(), sizeof(topic));
        strncpy(mqtt_username, webServer.arg("mqtt_username").c_str(), sizeof(mqtt_username));
        strncpy(mqtt_password, webServer.arg("mqtt_password").c_str(), sizeof(mqtt_password));
        mqtt_port = webServer.arg("mqtt_port").toInt();
        fanPin = webServer.arg("fanPin").toInt();
        tempThreshold = webServer.arg("tempThreshold").toFloat();
        fanOffDelay = webServer.arg("fanOffDelay").toInt();
        authToken = webServer.arg("authToken");

        // Save settings to NVS
        saveSettingsToNvs();

        // Perform any reset actions
        if (webServer.hasArg("action")) {
            String action = webServer.arg("action");
            if (action == "Save") {
                // Just save the settings
                Serial.println("Settings Saved.");
            } else if (action == "Save and Restart") {
                // Save the settings and schedule a restart
                Serial.println("Settings Saved. Device will restart in 5 seconds.");
                delay(5000);    // Wait for 5 seconds
                ESP.restart();  // Restart the ESP32
            }
        }
        // Redirect back to config page or indicate that the settings have been saved
        webServer.sendHeader("Location", "/config");
        webServer.send(303);

        // Redirect back to config page or a confirmation page
        webServer.sendHeader("Location", "/config");
        webServer.send(303);
    } else {
        webServer.send(405);
    }
}

void handleSetFanSpeedAPI() {
    // Check if API access is disabled
    if (authToken == "-1") {
        webServer.send(403, "text/plain", "API Access is disabled");
        return;
    }

    // Proceed with the existing logic
    if (webServer.hasArg("token") && webServer.hasArg("speed")) {
        String token = webServer.arg("token");
        if (token == authToken) {
            int speed = webServer.arg("speed").toInt();
            // Add your code to set the fan speed here
            manualFanSpeed = speed;
            webServer.send(200, "text/plain", "Fan speed set to " + String(manualFanSpeed) + "%");
        } else {
            webServer.send(403, "text/plain", "Forbidden: Invalid Token");
        }
    } else {
        webServer.send(400, "text/plain", "Bad Request: Missing 'token' or 'speed' parameter");
    }
}
