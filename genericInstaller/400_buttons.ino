#include "config.h"

int bothPressed = 0;
int firstPressed = 0;
int secondPressed = 0;

bool executedUnlock = false;
bool showWarning = false;

void setupButtons() {
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
}

void loopButtons() {
  if (bothPressed > 0 && digitalRead(BTN_1) == HIGH && digitalRead(BTN_2) == HIGH) {
    Serial.println("Both buttons released after " + String(millis() - bothPressed) + " ms");
    bothPressed = 0;
    executedUnlock = false;
    showWarning = false;
    printHome();
  } else if (firstPressed > 0 && digitalRead(BTN_1) == HIGH) {
    // button 1 released after
    const int releaseTime = millis() - firstPressed;
    Serial.println("Button 1 released after " + String(releaseTime) + " ms");
    firstPressed = 0;
    switch (currentScreen) {
      case SCREEN_HOME:
        currentMenuItem++;
        // last screen in enum is SCREEN_INFO, first is SCREEN_QR
        if (currentMenuItem > SCREEN_INFO) currentMenuItem = SCREEN_QR;
        break;
      case SCREEN_QR:
        Serial.println("Show QR Code press");
        break;
      case SCREEN_SETTINGS:
        if (releaseTime < 500) {
          currentSetting++;
          if (currentSetting > SETTING_BOOT_LOCK) currentSetting = SETTING_ENABLE_BLINK;
        } else {
          // long press, toggle setting
          switch (currentSetting) {
            case SETTING_ENABLE_BLINK:
              enable_blink = !enable_blink;
              Serial.println("Enable blink: " + String(enable_blink));
              break;
            case SETTING_BOOT_LOCK:
              if (config_boot_lock == 1) {
                config_boot_lock = 0;
              } else {
                config_boot_lock = 1;
              }
              Serial.println("Boot lock: " + String(config_boot_lock));
              writeConfig();
              break;
          }
        }
        break;
        Serial.println("Show QR Code press");
        break;
      default:
        Serial.println("Other screen press 1");
        break;
    }
    printHome();
  } else if (secondPressed > 0 && digitalRead(BTN_2) == HIGH) {
    Serial.println("Button 2 released after " + String(millis() - secondPressed) + " ms");
    secondPressed = 0;
    switch (currentScreen) {
      // toggle through menu items
      case SCREEN_HOME:
        Serial.println("Go to selected menu item: " + String(currentMenuItem));
        currentScreen = currentMenuItem;
        break;
      // cancel and go back to home screen
      default:
        Serial.println("Cancel from screen " + String(currentScreen));
        currentScreen = SCREEN_HOME;
        break;
    }
    printHome();
  }

  if (digitalRead(BTN_1) == LOW && digitalRead(BTN_2) == LOW) {
    if (bothPressed == 0) {
      Serial.println("Both buttons pressed.");
      bothPressed = millis();
    }
    if (!executedUnlock && millis() - bothPressed >= 5000) {
      Serial.println("Both buttons pressed for more than 5 secs.");
      if (config_boot_lock == 1) {
        Serial.println("Disable boot lock.");
        config_boot_lock = 0;
      } else {
        Serial.println("Enable boot lock.");
        config_boot_lock = 1;
      }
      executedUnlock = true;
      writeConfig();
      printHome();
    }
    if (!showWarning && millis() - bothPressed >= 7000) {
      Serial.println("Show delete config in 3 seconds warning...");
      printDeleteWarning();
      showWarning = true;
    }
    if (millis() - bothPressed >= 10000) {
      Serial.println("Delete config and reset device.");
      deleteConfig();
    }
  } else if (digitalRead(BTN_1) == LOW) {
    if (firstPressed == 0) {
      Serial.println("Button 1 pressed.");
      firstPressed = millis();
    }
  } else if (digitalRead(BTN_2) == LOW) {
    if (secondPressed == 0) {
      Serial.println("Button 2 pressed.");
      secondPressed = millis();
    }
  }
}
