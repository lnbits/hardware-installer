#ifndef CONFIG_H
#define CONFIG_H

#define NAME "Generic LNbits Device"
#define VERSION "v0.1.0"
#define INSTALLER_URL "https://hwi.lnbits.com/"

// time in seconds the installer can read/write config
#define BOOTUP_TIMEOUT 1
#define CONFIG_FILE "/config.txt"

// uncomment if you dont want to use the configuration file
//#define HARDCODED

// device specific configuration / defaults
#define CONFIG_WIFI_SSID "my_wifi_ssid"
#define CONFIG_WIFI_PASSWORD "my_wifi_password"


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

#endif
