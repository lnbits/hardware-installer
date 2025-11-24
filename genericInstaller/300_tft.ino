#ifndef TFT
void setupTFT() {}
void printTFT(String message, int x, int y) {}
void clearTFT() {}
void printHome() {}
void printConfig() {}
void printBoot() {}
void printDeleteWarning() {}
void printInstaller() {}
void printMenu() {}
void printQrCode(String data) {}
void printSettings() {}
void printInfo() {}
#else
#include "config.h"
#include <TFT_eSPI.h>
#include <QRCodeGenerator.h>

#define LINE_HEIGHT 20
#define PADDING_X 12
#define PADDING_Y 21
#define HEADER_PADDING_Y 4
#define VERSION_PADDING_X 200

TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);
void setupTFT() {
  tft.init();
  Serial.println("TFT: " + String(TFT_WIDTH) + "x" + String(TFT_HEIGHT));
  tft.setRotation(1);
  tft.fillScreen(TFT_PURPLE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(PADDING_X, 36);
  tft.println("powered by");
  tft.setTextSize(5);
  tft.setCursor(PADDING_X, 58);
  tft.println("LNbits");
  delay(200);
}
void printTFT(String message, int x, int y) {
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(x, y);
  tft.println(message);
}
void clearTFT() {
  tft.fillScreen(TFT_BLACK);
}
void printInstaller() {
    tft.setTextSize(2);
    tft.setCursor(PADDING_X, 52);
    tft.println(String(VERSION));
    tft.setCursor(PADDING_X, 72);
    tft.println("use webinstaller");
    tft.setCursor(PADDING_X, 92);
    tft.println("to configure");
    tft.setCursor(PADDING_X, 116);
    tft.setTextSize(1);
    tft.println(String(INSTALLER_URL));
}
void printConfig() {
    tft.fillScreen(TFT_PURPLE);
    tft.setTextSize(4);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(PADDING_X, 14);
    tft.println("CONFIG");
    printInstaller();
}
void printBoot() {
    tft.fillScreen(TFT_BLUE);
    tft.setTextSize(4);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(PADDING_X, 14);
    tft.println("BOOT");
    printInstaller();
}
void drawKey(int x, int y) {
    tft.fillCircle(x + 3, y + 3, 3, TFT_YELLOW);
    tft.fillCircle(x + 3, y + 3, 1, TFT_BLACK);
    tft.fillRect(x + 6, y + 3, 8, 2, TFT_YELLOW);
    // add v like tooth
    tft.fillRect(x + 10, y + 4, 2, 2, TFT_YELLOW);
    tft.fillRect(x + 12, y + 6, 2, 2, TFT_YELLOW);
    tft.fillRect(x + 14, y + 4, 2, 2, TFT_YELLOW);

}
void drawWifiBars(int x, int y, int quality) {
    tft.setTextSize(1);
    tft.setCursor(x, y);
    tft.setTextColor(TFT_WHITE);
    tft.println(String(quality) + "%");
    for (int8_t i = 0; i < 4; i++) {
      for (int8_t j = 0; j < 2 * (i + 1); j++) {
        if (quality > i * 25 || j == 0) {
          tft.fillRect(x + 21 + i * 5, y + 6 - j, 4, 1, TFT_GREEN);
        } else {
          tft.fillRect(x + 21 + i * 5, y + 6 - j, 4, 1, TFT_DARKGREY);
        }
      }
    }
}
void drawBattery(int x, int y, int percentage) {
    if (percentage > 100) percentage = 100; // clamp to 100
    tft.setTextSize(1);
    tft.setCursor(x, y);
    tft.setTextColor(TFT_WHITE);
    tft.println(String(percentage) + "%");
    x += 22;
    tft.drawRect(x, y, 23, 7, TFT_WHITE);
    tft.fillRect(x + 23, y + 1, 3, 4, TFT_WHITE);
    int fillWidth = map(percentage, 0, 100, 0, 21);
    if (percentage > 20) {
        tft.fillRect(x + 1, y + 1, fillWidth, 5, TFT_GREEN);
    } else {
        tft.fillRect(x + 1, y + 1, fillWidth, 5, TFT_RED);
    }
}
void drawUsb(int x, int y) {
    tft.setTextSize(1);
    tft.setCursor(x, y);
    tft.setTextColor(TFT_BLUE);
    tft.println("USB");
    tft.setTextColor(TFT_WHITE);
}
void printHome() {

    if (currentScreen == SCREEN_QR) {
        printQrCode("dni@lnbits.com");
        return;
    }

    clearTFT();

    int footerY = TFT_WIDTH - 10;
    int versionX = TFT_HEIGHT - 42;

    // draw footer
    tft.setTextSize(1);
    tft.setTextColor(TFT_PURPLE);
    tft.setCursor(PADDING_X, footerY);
    tft.println(String(NAME));
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(versionX, footerY);
    tft.println(String(VERSION));

    int x = TFT_HEIGHT;
    // draw top menu
    if (battery <= 100 && battery > 0) {
        x -= 51;
        drawBattery(x, HEADER_PADDING_Y, battery);
    } else {
        x -= 24;
        drawUsb(x, HEADER_PADDING_Y);
    }
    if (wifi_connected) {
        x -= 48;
        int8_t quality = getWifiQuality();
        drawWifiBars(x, HEADER_PADDING_Y, quality);
    }
    if (config_boot_lock == 1) {
        x -= 24;
        drawKey(x, HEADER_PADDING_Y);
    }
    // draw current screen
    switch (currentScreen) {
      case SCREEN_HOME:
        printMenu();
        break;
      case SCREEN_SETTINGS:
        printSettings();
        break;
      case SCREEN_INFO:
        printInfo();
        break;
      default:
        Serial.println("Switches to screen" + String(currentScreen));
        break;
    }
}
void printDeleteWarning() {
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE);
    printTFT("HOLD", PADDING_X, 21);
    printTFT("3 MORE SECONDS", PADDING_X, 42);
    printTFT("TO ERASE", PADDING_X, 63);
    printTFT("CONFIG", PADDING_X, 84);
}
void printMenu() {
  // Draw menu items
  int y = PADDING_Y;
  tft.setTextSize(2);
  tft.setCursor(PADDING_X, y);
  if (currentMenuItem == SCREEN_QR) tft.setTextColor(TFT_YELLOW); else tft.setTextColor(TFT_WHITE);
  tft.println("Show QR Code");

  y += LINE_HEIGHT;
  tft.setCursor(PADDING_X, y);
  if (currentMenuItem == SCREEN_SETTINGS) tft.setTextColor(TFT_YELLOW); else tft.setTextColor(TFT_WHITE);
  tft.println("Settings");

  y += LINE_HEIGHT;
  tft.setCursor(PADDING_X, y);
  if (currentMenuItem == SCREEN_INFO) tft.setTextColor(TFT_YELLOW); else tft.setTextColor(TFT_WHITE);
  tft.println("Device Info");
}
void printQrCode(String data)
{
  const int brightness = 200; // 0-255
  uint16_t qrScreenBgColour = tft.color565(brightness, brightness, brightness);
  tft.fillScreen(qrScreenBgColour);
  const char *qrDataChar = data.c_str();

  QRCode qrcoded;
  uint8_t qrcodeData[qrcode_getBufferSize(20)];
  qrcode_initText(&qrcoded, qrcodeData, 6, 0, qrDataChar);

  unsigned int pixSize = 3;
  unsigned int offsetTop = 5;
  unsigned int offsetLeft = 65;

#ifdef TDISPLAY_S3
  pixSize = 4;
  offsetLeft = 80;
#endif

  for (uint8_t y = 0; y < qrcoded.size; y++)
  {
    for (uint8_t x = 0; x < qrcoded.size; x++)
    {
      if (qrcode_getModule(&qrcoded, x, y))
      {
        tft.fillRect(offsetLeft + pixSize * x, offsetTop + pixSize * y, pixSize, pixSize, TFT_BLACK);
      }
      else
      {
        tft.fillRect(offsetLeft + pixSize * x, offsetTop + pixSize * y, pixSize, pixSize, qrScreenBgColour);
      }
    }
  }
}
void printInfo() {
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(PADDING_X, 14);
    tft.println("Info");
    tft.setTextSize(2);
    tft.setCursor(PADDING_X, 52);
    tft.println("WiFi: " + String(config_wifi_ssid));
    tft.setCursor(PADDING_X, 72);
    tft.println("IP: " + String(WiFi.localIP().toString()));
    tft.setCursor(PADDING_X, 92);
    tft.println("LED Pin: " + String(config_led_pin));
}
void printSettings() {
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(PADDING_X, 14);
    tft.println("Settings");
    tft.setTextSize(2);
    int y = 52;
    tft.setCursor(PADDING_X, y);
    if (currentSetting == SETTING_ENABLE_BLINK) tft.setTextColor(TFT_YELLOW); else tft.setTextColor(TFT_WHITE);
    tft.println("Enable Blink: " + String(enable_blink ? "YES" : "NO"));
    y += LINE_HEIGHT;
    tft.setCursor(PADDING_X, y);
    if (currentSetting == SETTING_BOOT_LOCK) tft.setTextColor(TFT_YELLOW); else tft.setTextColor(TFT_WHITE);
    tft.println("Bootlock: " + String(config_boot_lock == 1 ? "LOCKED" : "UNLOCKED"));
}
#endif
