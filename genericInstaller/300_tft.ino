#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);
tft.init();
tft.setRotation(1);
tft.invertDisplay(false);
tft.fillScreen(TFT_GREEN);
tft.setTextColor(ftcolor, bgcolor);
tft.setTextSize(3);
tft.setCursor((horizontalOffset - tft.textWidth(text1)) / 2, verticalOffset);
