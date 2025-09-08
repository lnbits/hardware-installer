#include "config.h"

#ifdef HARDCODED
void setupConfig(){
    Serial.println("Setting hardcoded values...");
    Serial.println("LED pin: " + String(CONFIG_LED_PIN));
    Serial.println("WIFI SSID: " + CONFIG_WIFI_SSID);
    Serial.println("WIFI password: " + CONFIG_WIFI_PASSWORD);
    config_led_pin = CONFIG_LED_PIN;
    config_wifi_ssid = CONFIG_WIFI_SSID;
    config_wifi_password = CONFIG_WIFI_PASSWORD;
    config_boot_lock = 0;
}
void writeConfig() {
    Serial.println("Cannot write config in HARDCODED mode.");
    return "";
}
void deleteConfig() {
    Serial.println("Cannot delete config in HARDCODED mode.");
    return "";
}
#else
#include <FS.h>
#include <SPIFFS.h>

void setupConfig(){
    SPIFFS.begin(true);
    String fileContent = readConfig();
    // file does not exist, so we will enter endless config mode
    if (fileContent == "") {
        Serial.println("Config file does not exist.");
        executeConfigForever();
    }

    config_led_pin = getConfigInt(fileContent, "led_pin", CONFIG_LED_PIN);
    config_wifi_ssid = getConfigValue(fileContent, "wifi_ssid", CONFIG_WIFI_SSID);
    config_wifi_password = getConfigValue(fileContent, "wifi_password", CONFIG_WIFI_PASSWORD);
    config_boot_lock = getConfigInt(fileContent, "boot_lock", 0);

    if (config_boot_lock == 0) {
      executeConfigBoot();
    }
}

void writeConfig() {
    String data = "";
    data += "led_pin=" + String(config_led_pin) + "\n";
    data += "wifi_ssid=" + config_wifi_ssid + "\n";
    data += "wifi_password=" + config_wifi_password + "\n";
    data += "boot_lock=" + String(config_boot_lock) + "\n";
    if (config_boot_lock == 0) {
      Serial.println("Writing config:\n" + data);
    }
    SPIFFS.begin(true);
    File paramFile = SPIFFS.open(CONFIG_FILE, FILE_WRITE);
    if (!paramFile) {
        Serial.println("Failed to open config file for writing.");
        return;
    }
    paramFile.print(data);
    paramFile.close();
    Serial.println("Config written.");
}

// format: key=value\n
String getConfigValue(String &fileContent, const char* name, String defaultValue) {
    int index = fileContent.indexOf(String(name) + "=");
    if (index == -1) {
        if (config_boot_lock == 0) {
          Serial.println("Config value for " + String(name) + " not found. Using default: " + defaultValue);
        }
        return defaultValue;
    }
    int endIndex = fileContent.indexOf("\n", index);
    if (endIndex == -1) {
        endIndex = fileContent.length();
    }
    String value = fileContent.substring(index + strlen(name) + 1, endIndex);
    if (config_boot_lock == 0) {
      Serial.println(String(name) + "=" + value);
    }
    return value;
}

int getConfigInt(String &fileContent, const char* name, int defaultValue) {
    String str = getConfigValue(fileContent, name, String(defaultValue));
    return str.toInt();
}

String readConfig() {
    File paramFile = SPIFFS.open(CONFIG_FILE, FILE_READ);
    if (!paramFile) {
        return "";
    }
    String fileContent = paramFile.readString();
    if (fileContent == "") {
        return "";
    }
    paramFile.close();
    return fileContent;
}

void executeConfigBoot() {
    Serial.println("Entering boot mode. Waiting for " + String(BOOTUP_TIMEOUT) + " seconds.");
    printBoot();
    int counter = BOOTUP_TIMEOUT + 1;
    while (counter-- > 0) {
        if (Serial.available() == 0) {
            delay(1000);
            continue;
        }
        Serial.println();
        // if we get serial data in the first 5 seconds, we will enter config mode
        counter = 0;
        executeConfigForever();
    }
    Serial.println("Exiting boot mode.");
    Serial.print("Welcome to the LNbits " + String(NAME) + "!");
    Serial.println(" (" + String(VERSION) + ")");
    printHome(false);
}

void executeConfigForever() {
    Serial.println("Entering config mode. until we receive /config-done.");
    printConfig();
    bool done = false;
    while (true) {
        done = executeConfig();
        if (done) {
            Serial.println("Exiting config mode.");
            ESP.restart();
            return;
        }
    }
}

void deleteConfig() {
    SPIFFS.remove(CONFIG_FILE);
    ESP.restart();
}

bool executeConfig() {
  if (Serial.available() == 0) return false;
  String data = Serial.readStringUntil('\n');
  Serial.println("received serial data: " + data);
  if (data == "/config-done") {
    delay(1000);
    return true;
  }
  if (data == "/file-remove") {
    deleteConfig();
  }
  if (data.startsWith("/file-append")) {
    File file = SPIFFS.open(CONFIG_FILE, FILE_APPEND);
    if (!file) {
      file = SPIFFS.open(CONFIG_FILE, FILE_WRITE);
    }
    if (!file) {
      Serial.println("Failed to open file for writing.");
    }
    if (file) {
      int pos = data.indexOf(" ");
      String _data = data.substring(pos + 1);
      file.println(_data);
      file.close();
    }
  }
  if (data.startsWith("/file-read")) {
    File file = SPIFFS.open(CONFIG_FILE, "r");
    if (file) {
      while (file.available()) {
        String line = file.readStringUntil('\n');
        Serial.println("/file-send " + line);
      }
      file.close();
      Serial.println("/file-done");
    }
    return false;
  }
  return false;
}
#endif
