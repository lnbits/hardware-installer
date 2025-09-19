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


void setup() {
    Serial.begin(115200);
    setupTFT();
    setupConfig();
    setupWifi();
    setupButtons();
    setupBattery();

    // setup led
    pinMode(config_led_pin, OUTPUT);
}

void loop() {
    loopWifi();
    loopButtons();
    loopBattery();

    // blink led or backlight on tdisplay
    if (enable_blink) blinkLed(config_led_pin);
}

int lastBlink = 0;

void blinkLed(int pin) {
    int state = digitalRead(pin);
    int wait = 100;
    if (state == HIGH) {
        wait = 5000;
    }
    if (millis() - lastBlink > wait) {
        lastBlink = millis();
        digitalWrite(pin, !state);
    }
}
