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
    printHome(true);
  } else if (firstPressed > 0 && digitalRead(BTN_1) == HIGH) {
    Serial.println("Button 1 released after " + String(millis() - firstPressed) + " ms");
    firstPressed = 0;
  } else if (secondPressed > 0 && digitalRead(BTN_2) == HIGH) {
    Serial.println("Button 2 released after " + String(millis() - secondPressed) + " ms");
    secondPressed = 0;
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
      printHome(true);
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
