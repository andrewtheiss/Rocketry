#include "TouchScreenTFT.h"

// Constructor implementation
// ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
// XPT2046_Touchscreen touch(TOUCH_CS);
TouchScreenTFT::TouchScreenTFT() : tft(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO), touch(TOUCH_CS) {}

void TouchScreenTFT::init() {
  Serial.begin(9600);
  touch.begin();
  Serial.begin(9600);
  tft.begin();
  redrawScreenSometimes();
}

void TouchScreenTFT::redrawScreenSometimes() {
  Serial.println("Redrawing screen");
  tft.setRotation(3);

  tft.fillScreen(ILI9341_BLACK);
  delay(100); // Short delay after clearing the screen

  tft.fillRect(BUTTON_1_RED_X, BUTTON_1_RED_Y, BUTTON_W, BUTTON_H, ILI9341_RED);
  tft.fillRect(BUTTON_2_GREEN_X, BUTTON_2_GREEN_Y, BUTTON_W, BUTTON_H, ILI9341_GREEN);
  tft.fillRect(BUTTON_3_BLUE_X, BUTTON_3_BLUE_Y, BUTTON_W, BUTTON_H, ILI9341_BLUE);
  tft.fillRect(BUTTON_4_YELLOW_X, BUTTON_4_YELLOW_Y, BUTTON_W, BUTTON_H, ILI9341_YELLOW);
  delay(10); // Short delay after clearing the screen
}

void TouchScreenTFT::loop() {
  // Check for touch events
  checkTouch();
}

unsigned int TouchScreenTFT::checkTouch() {
  // Example raw touch values (minimum and maximum)
  int touchXmin = 300;  // Replace with your minimum x value
  int touchXmax = 3834; // Replace with your maximum x value
  int touchYmin = 240;  // Replace with your minimum y value
  int touchYmax = 3722; // Replace with your maximum y value
  touch.begin();
  if (touch.touched()) {
    // Serial.print("button touched");
    TS_Point p = touch.getPoint();
    // Serial.print("x:");
    // Serial.print(p.x);
    // Serial.print(" y:");
    // Serial.print(p.y);
    // Serial.print("     ");

    // Adjusted map function
    p.x = map(p.x, touchXmin, touchXmax, 0, 320);
    p.y = map(p.y, touchYmin, touchYmax, 0, 240);

  Serial.println();
    Serial.print("ax:");
    Serial.print(p.x);
    Serial.print(" ay:");
    Serial.print(p.y);
    Serial.print("     ");


    // Check if within button boundaries and call respective functions
    if (p.x >= BUTTON_1_RED_X && p.x < (BUTTON_1_RED_X + BUTTON_W)) {
      if (p.y >= BUTTON_1_RED_Y && p.y < (BUTTON_1_RED_Y + BUTTON_H)) {
        button1Function();
        return 1;
      }
    }    
    // Check if within button boundaries and call respective functions
    if (p.x >= BUTTON_4_YELLOW_X && p.x < (BUTTON_4_YELLOW_X + BUTTON_W)) {
      if (p.y >= BUTTON_4_YELLOW_Y && p.y < (BUTTON_4_YELLOW_Y + BUTTON_H)) {
        button4Function();
        return 1;
      }
    }
  }
    delay(100);
  return 0;
  // Your existing loop code for handling touch events
}

// Implementation of private functions (button1Function, button2Function, etc.)
void TouchScreenTFT::button1Function() {

  Serial.println();
  Serial.print("button 1 red");
  Serial.println();
}

void TouchScreenTFT::button2Function() {
  // Implementation
}

void TouchScreenTFT::button3Function() {
  // Implementation
}

void TouchScreenTFT::button4Function() {
  // Implementation
  redrawScreenSometimes();
}
