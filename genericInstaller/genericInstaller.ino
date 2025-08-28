#include "config.h"

// globals
String config_ssid;
String config_password;
int config_led_pin;
int config_boot_lock;

void setup() {
    Serial.begin(115200);
    setupTFT();
    setupConfig();
    setupWifi();
    setupButtons();

    // setup led
    pinMode(config_led_pin, OUTPUT);
}

void loop() {
    loopWifi();
    loopButtons();

    // blink led or backlight on tdisplay
    blinkLed(config_led_pin);
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
