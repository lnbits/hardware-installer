#ifndef TFT
void setupTFT() {}
void printTFT(String message, int x, int y) {}
void clearTFT() {}
void printHome() {}
void printConfig() {}
void printBoot() {}
void printDeleteWarning() {}
#else
#include "config.h"
#include <TFT_eSPI.h>

#define LINE_HEIGHT 20
#define PADDING_X 12
#define PADDING_Y 21
#define HEADER_PADDING_Y 4
#define FOOTER_PADDING_Y 123
#define VERSION_PADDING_X 200

TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);
void setupTFT() {
  tft.init();
  Serial.println("TFT: " + String(TFT_WIDTH) + "x" + String(TFT_HEIGHT));
  Serial.println("TFT pin MISO: " + String(TFT_MISO));
  Serial.println("TFT pin CS: " + String(TFT_CS));
  Serial.println("TFT pin MOSI: " + String(TFT_MOSI));
  Serial.println("TFT pin SCLK: " + String(TFT_SCLK));
  Serial.println("TFT pin DC: " + String(TFT_DC));
  Serial.println("TFT pin RST: " + String(TFT_RST));
  Serial.println("TFT pin BL: " + String(TFT_BL));
  tft.setRotation(1);
  tft.invertDisplay(true);
  tft.fillScreen(TFT_PURPLE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(PADDING_X, 36);
  tft.println("powered by");
  tft.setTextSize(5);
  tft.setCursor(PADDING_X, 58);
  tft.println("LNbits");
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

void printHome() {
    clearTFT();
    tft.setTextSize(1);
    tft.setTextColor(TFT_PURPLE);
    tft.setCursor(PADDING_X, FOOTER_PADDING_Y);
    tft.println(String(NAME));
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(VERSION_PADDING_X, FOOTER_PADDING_Y);
    tft.println(String(VERSION));
    if (wifi_connected) {
        printTFT("WiFi connected", PADDING_X, PADDING_Y);
        int8_t quality = getWifiQuality();
        drawWifiBars(196, HEADER_PADDING_Y, quality);
    } else {
        printTFT("No WiFi", PADDING_X, PADDING_Y);
        drawWifiBars(196, HEADER_PADDING_Y, 0);
    }
    if (config_boot_lock == 1) {
        printTFT("BOOT LOCKED", PADDING_X, PADDING_Y + LINE_HEIGHT);
        drawKey(172, HEADER_PADDING_Y);
    } else {
        printTFT("BOOT UNLOCKED", PADDING_X, PADDING_Y + LINE_HEIGHT);
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
#endif
