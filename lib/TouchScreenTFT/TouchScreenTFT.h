#ifndef TouchScreenTFT_h
#define TouchScreenTFT_h

// For optimized ILI9341_t3 library
#define TFT_DC      9
#define TFT_CS      10
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI    11
#define TFT_SCLK    13  // Cant change this
#define TFT_MISO    12 
#define TOUCH_CS    8   // Chip select pin for the touchscreen

// ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
// XPT2046_Touchscreen touch(TOUCH_CS);


// Define button coordinates and sizes
#define BUTTON_W  100
#define BUTTON_H  50
#define BUTTON_SPACING_X 20
#define BUTTON_SPACING_Y 20
#define BUTTON_Y 100  // Starting Y coordinate of buttons

#define BUTTON_1_RED_X 0
#define BUTTON_1_RED_Y 0
#define BUTTON_2_GREEN_X 160
#define BUTTON_2_GREEN_Y 0
#define BUTTON_3_BLUE_X 0
#define BUTTON_3_BLUE_Y 120
#define BUTTON_4_YELLOW_X 160
#define BUTTON_4_YELLOW_Y 120


#include <Arduino.h>
#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>

class TouchScreenTFT {
  public:
    TouchScreenTFT(); // Constructor
    void init(); // Initialize the touchscreen and TFT display
    void checkTouch(); // Check for touch and handle it

  private:
    ILI9341_t3 tft;
    XPT2046_Touchscreen touch;
    void button1Function();
    void button2Function();
    void button3Function();
    void button4Function();
};

#endif
