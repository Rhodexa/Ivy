#ifndef IVY_WIFI_H
#define IVY_WIFI_H

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "ivy_nuts_and_bolts.h"

#define BUTTON_PIN 13   // Button to enter AP mode
#define LED_PIN 2       // LED indicator for AP mode
#define AP_TIMEOUT 120000 // 2 minutes in milliseconds

Preferences preferences;
WebServer server(80);

bool apMode = false;
unsigned long apStartTime = 0;
String ivy_wifi_last_ip = "---.---.---.---";

// ======== Function: Connect to Wi-Fi =========
void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  String ssid = preferences.getString("ssid", "");
  String password = preferences.getString("password", "");

  if (ssid.length() > 0) {
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to Wi-Fi: "); Serial.println(ssid);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWi-Fi Connected!");
      Serial.print("Local IP: "); Serial.println(WiFi.localIP());
      ivy_wifi_last_ip = WiFi.localIP().toString();
    } else {
      Serial.println("\nWi-Fi Connection Failed!");
    }
  } else {
    Serial.println("No Wi-Fi credentials found.");
  }
}

// ======== Function: Start AP Mode =========
void startAPMode() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Ivy Config");
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP Mode Started. Connect to 'ESP32_Config' and visit: ");
  Serial.println(apIP);

  digitalWrite(LED_PIN, HIGH); // Turn ON AP mode indicator LED

  apMode = true;
  apStartTime = millis();

  // Serve the Wi-Fi config page
  server.on("/", HTTP_GET, []() {
    String page = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Configuración</title>";
    // Include Bootstrap from a CDN
    page += "<link href='https://cdn.jsdelivr.net/npm/bootstrap@4.5.2/dist/css/bootstrap.min.css' rel='stylesheet' />";
    page += "<script src='https://code.jquery.com/jquery-3.5.1.slim.min.js'></script>";
    page += "<script src='https://cdn.jsdelivr.net/npm/bootstrap@4.5.2/dist/js/bootstrap.bundle.min.js'></script>";
    page += "</head><body>";

    page += "<div class='container mt-5'>";
      page += "<h2>IP Local: " + ivy_wifi_last_ip + "</h2><hr>";      
      page += "<h2>Wi-Fi Setup</h2>";
      // Start Wi-Fi Scan
      int numNetworks = WiFi.scanNetworks();
      if (numNetworks == 0) {
        page += "<div class='alert alert-warning'>No se pudo encontrar ninguna red Wi-Fi!</div>";
      } else {
        page += "<form action='/set' method='POST' class='form-group'>";
        page += "<label for='ssid'>Seleccioná tu red Wi-Fi:</label>";
        page += "<select name='ssid' id='ssid' class='form-control'>";
        
        for (int i = 0; i < numNetworks; i++) {
          page += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + " (" + 
                  String(WiFi.RSSI(i)) + " dBm)</option>";
        }
        
        page += "</select><br><br>";
        page += "<label for='password'>Contraseña:</label>";
        page += "<input type='password' name='password' id='password' class='form-control' required><br><br>";
        page += "<button type='submit' class='btn btn-primary'>Guardar</button>";
        page += "</form>";
      }
      page += "<div>" + String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec) + "</div>";

    page += "</div>";  // End container
    page += "</body></html>";
    
    server.send(200, "text/html", page);
  });



  server.on("/set", HTTP_POST, []() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    if (ssid.length() > 0) {
      preferences.putString("ssid", ssid);
      preferences.putString("password", password);
      server.send(200, "text/html", "<h3>Wi-Fi Saved. Restarting...</h3>");
      delay(2000);
      ESP.restart();
    } else {
      server.send(400, "text/html", "<h3>Invalid SSID</h3>");
    }
  });

  server.begin();
}

// ======== Function: Button Handler =========
volatile bool startAPFlag = false;  // Flag to signal AP mode start

void IRAM_ATTR handleButtonPress() {
  static unsigned long pressTime = 0;
  
  if (digitalRead(BUTTON_PIN) == LOW) { // Button pressed
    pressTime = millis();
  } else { // Button released
    if (millis() - pressTime > 3000) { // Long press detected
      startAPFlag = true;  // Set flag instead of starting AP mode directly
    }
  }
}


void ivy_wifi_begin(){
  preferences.begin("wifi", false);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);  // Set GPIO2 as output
  digitalWrite(LED_PIN, LOW); // Ensure LED is OFF at startup

  attachInterrupt(BUTTON_PIN, handleButtonPress, CHANGE);
  connectToWiFi();
}

// ======== Function: Auto-shutdown AP Mode =========
void ivy_wifi_loop() {

  server.handleClient();

  if (startAPFlag) {
    startAPFlag = false;  // Reset flag
    startAPMode();        // Start AP mode safely in the main loop
  }

  if (apMode) {
    if (millis() - apStartTime > AP_TIMEOUT) { // If AP mode is running for 2 minutes
      Serial.println("Timeout: Exiting AP mode.");
      digitalWrite(LED_PIN, LOW); // Turn OFF AP mode indicator LED
      ESP.restart();
    }
  }
}


#endif
