#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

int config_led_pin;
String config_ssid;
String config_password;


void setup() {
    Serial.begin(115200);

    setupConfig();
    setupWifi();
    pinMode(config_led_pin, OUTPUT); // To blink on board LED
}

void loop() {
    loopWifi();
    blinkLed(config_led_pin);
}

void blinkLed(int pin) {
    digitalWrite(pin, LOW);
    delay(2000);
    digitalWrite(pin, HIGH);
    delay(2000);
}
