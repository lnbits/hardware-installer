#include "config.h"

#include <WiFi.h>

int wifiReconnectDelay = 10000;
int wifiLastReconnectAttempt = 0;

void setupWifi() {
    printHome();
    Serial.println("Connecting to WiFi...");
    WiFi.begin(config_wifi_ssid.c_str(), config_wifi_password.c_str());
}

void loopWifi() {
    if (millis() - wifiLastReconnectAttempt >= wifiReconnectDelay && WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected! Reconnecting...");
        wifiLastReconnectAttempt = millis();
        wifi_connected = false;
        printHome();
        WiFi.reconnect();
    } else if (WiFi.status() == WL_CONNECTED) {
        static bool wasConnected = false;
        if (!wasConnected) {
            Serial.println("WiFi connected! ip: " + WiFi.localIP().toString());
            wifi_connected = true;
            printHome();
            wasConnected = true;
        }
    }
}

// converts the dBm to a range between 0 and 100%
int8_t getWifiQuality() {
  int32_t dbm = WiFi.RSSI();
  if (dbm <= -100) {
    return 0;
  } else if (dbm >= -50) {
    return 100;
  } else {
    return 2 * (dbm + 100);
  }
}
