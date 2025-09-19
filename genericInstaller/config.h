#pragma once

#define NAME "Generic LNbits Device"
#define VERSION "v0.1.0"
#define INSTALLER_URL "https://hwi.lnbits.com/"

// time in seconds the installer can read/write config
#define BOOTUP_TIMEOUT 2
#define CONFIG_FILE "/config.txt"

#define BATTERY_PIN 34

#ifdef ESP32
  #define BTN_1 0
  #define BTN_2 4
  #define CONFIG_LED_PIN 2
#endif

#ifdef TDISPLAY
  #define BTN_1 0
  #define BTN_2 35
  // blink backlight
  #define CONFIG_LED_PIN 4
#endif

enum Screen {
  SCREEN_HOME,
  SCREEN_QR,
  SCREEN_SETTINGS,
  SCREEN_INFO,
};

enum SettingsMenu {
    SETTING_ENABLE_BLINK,
    SETTING_BOOT_LOCK,
};
