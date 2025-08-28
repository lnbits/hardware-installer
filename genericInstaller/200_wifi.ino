#include "config.h"

#include <WiFi.h>

void setupWifi() {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(config_ssid.c_str(), config_password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
    }
    Serial.println("WiFi connected! ip: " + WiFi.localIP().toString());
    printHome(true);
}

void loopWifi() {
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected! Reconnecting...");
        printHome(false);
        WiFi.reconnect();
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
