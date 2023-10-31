#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

fs::SPIFFSFS &FlashFS = SPIFFS;
#define FORMAT_ON_FAIL true
#define PARAM_FILE "/elements.json"

struct KeyValue {
    String key;
    String value;
};


String config_ssid = "null";
String config_password= "null";

// Serial config
int portalPin = 4;

StaticJsonDocument<2500> doc;

void setupConfig() {
    readConfig(doc);
    if(config_ssid == "null"){
        config_ssid = getJsonValue(doc, "config_ssid");
        Serial.println("SSID (memory): " + config_ssid);
    }
    else{
        Serial.println("SSID (hardcoded): " + config_ssid);
    }
    if(config_password == "null"){
        config_password = getJsonValue(doc, "config_password");
        Serial.println("SSID password (memory): " + config_password);
    }
    else{
        Serial.println("SSID password (hardcoded): " + config_password);
    }
}

void setup() {
    Serial.begin(115200);
    bool triggerConfig = false;
    pinMode (2, OUTPUT); // To blink on board LED
    FlashFS.begin(FORMAT_ON_FAIL);
    int timer = 0;
    while (timer < 2000) {
        digitalWrite(2, HIGH);
        Serial.println("Detecting portalPin: " + String(touchRead(portalPin)));
        if (touchRead(portalPin) < 60) {
            triggerConfig = true;
            timer = 5000;
        }
        timer = timer + 100;
        delay(150);
        digitalWrite(2, LOW);
        delay(150);
    }

    setupConfig();

    if(triggerConfig == true || config_ssid == "" || config_ssid == "null"){
        Serial.println("Launch serial config");
        configOverSerialPort();
    }
    else{
        WiFi.begin(config_ssid.c_str(), config_password.c_str());
        Serial.print("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(500);
            digitalWrite(2, HIGH);
            Serial.print(".");
            delay(500);
            digitalWrite(2, LOW);
        }
    }
}

void loop() {
    while(WiFi.status() != WL_CONNECTED){
        Serial.println("WiFi disconnected");
        delay(500);
    }
    digitalWrite(2, LOW);
    delay(2000);
    digitalWrite(2, HIGH);
}
