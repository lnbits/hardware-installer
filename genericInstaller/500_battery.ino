#include "config.h"

int lastBatteryCheck = 0;
int checkBatteryInterval = 5000; // check battery every 5 seconds

void setupBattery() {
    battery = getBatteryPercentage();
    lastBatteryCheck = millis();
}

void loopBattery() {
    if (millis() - lastBatteryCheck > checkBatteryInterval) {
        lastBatteryCheck = millis();
        battery = getBatteryPercentage();
        printHome();
    }
}

unsigned int getBatteryPercentage()
{
  const float batteryMaxVoltage = 4.2;
  const float batteryMinVoltage = 3.73;
  const float inputVoltage = getInputVoltage();

  const float batteryAllowedRange = batteryMaxVoltage - batteryMinVoltage;
  const float batteryCurVAboveMin = inputVoltage - batteryMinVoltage;

  const int batteryPercentage = (int)(batteryCurVAboveMin / batteryAllowedRange * 100);
  Serial.printf("Voltage: %.2f V, percentage: %d%%\n", inputVoltage, batteryPercentage);
  return batteryPercentage;
}

float getInputVoltage()
{
  const uint16_t v1 = analogRead(BATTERY_PIN);
  return ((float)v1 / 4095.0f) * 2.0f * 3.3f * (1100.0f / 1000.0f);
}
