#include "config.h"

// config
String config_wifi_ssid;
String config_wifi_password;
int config_led_pin;
int config_boot_lock;

// app state
int battery = 0;
int wifi_connected = false;
int enable_blink = true;

// ui state
int currentMenuItem = SCREEN_QR;
int currentScreen = SCREEN_HOME;
int currentSetting = SETTING_ENABLE_BLINK;

uint16_t touchX, touchY;

void setup() {
    Serial.begin(115200);

    // LilyGo Bug enable battery fix
    #ifdef TDISPLAY_S3
      pinMode(PIN_ENABLE5V, OUTPUT);
      digitalWrite(PIN_ENABLE5V, HIGH);
    #endif

    setupTFT();
    setupConfig();
    setupWifi();
    setupButtons();
    setupTouch();
    setupBattery();
}

void loop() {
    loopWifi();
    loopButtons();
    loopTouch();
    loopBattery();
}
