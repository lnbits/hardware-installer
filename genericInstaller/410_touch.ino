#include "config.h"

void setupTouch() {
   #ifdef TOUCH
    uint16_t touchCalibration[5] = { 344, 3356, 242, 3616, 4 };
    // uint16_t touchCalibration[5] = { 286, 3534, 283, 3600, 6 };
    tft.setTouch(touchCalibration);
    Serial.println("Touch initialised");
   #endif
}

void loopTouch() {
   #ifdef TOUCH
   if (tft.getTouch(&touchX, &touchY)) {
       Serial.printf("Touch: %d, %d, %d\n", touchX, touchY, tft.getTouchRawZ());
   }
   #endif
}
