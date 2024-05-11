#include "TouchScreenTFT.h"

// Constructor implementation
// ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
// XPT2046_Touchscreen touch(TOUCH_CS);
TouchScreenTFT::TouchScreenTFT() : tft(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO), touch(TOUCH_CS, TOUCH_IRQ) {}

void TouchScreenTFT::init() {
  Serial.begin(9600);
  screenRedrawCounter = 0;
  touch.begin();
  tft.begin();
  redrawScreenSometimes();
  pinMode(TOUCH_IRQ, INPUT_PULLUP);  // Set as input with pull-up

}


void TouchScreenTFT::redrawScreenSometimes() {
  Serial.println("Redrawing screen");

  tft.fillScreen(ILI9341_BLACK);
  delay(100); // Short delay after clearing the screen

  tft.fillRect(BUTTON_1_RED_X, BUTTON_1_RED_Y, BUTTON_W, BUTTON_H, ILI9341_RED);
  tft.fillRect(BUTTON_2_GREEN_X, BUTTON_2_GREEN_Y, BUTTON_W, BUTTON_H, ILI9341_GREEN);
  tft.fillRect(BUTTON_3_BLUE_X, BUTTON_3_BLUE_Y, BUTTON_W, BUTTON_H, ILI9341_BLUE);
  tft.fillRect(BUTTON_4_YELLOW_X, BUTTON_4_YELLOW_Y, BUTTON_W, BUTTON_H, ILI9341_YELLOW);
  delay(10); // Short delay after clearing the screen
}

void TouchScreenTFT::updateScreen() {

  if (!(screenRedrawCounter%20)) {
    tft.setTextColor(ILI9341_RED);  // Set the text color
    tft.setCursor(0, 0);  // Set the cursor to the desired position
    tft.fillRect(BUTTON_1_RED_X, BUTTON_1_RED_Y, BUTTON_W, BUTTON_H,ILI9341_BLACK);
    tft.print("000000");  // Display the current number and increment it
    tft.setCursor(0, 0);  // Set the cursor to the desired position
    tft.fillRect(BUTTON_1_RED_X, BUTTON_1_RED_Y, BUTTON_W, BUTTON_H, ILI9341_RED);
    tft.setTextColor(ILI9341_GREEN);  // Set the text color
    tft.setTextSize(3);  // Set the text size
    tft.print(screenRedrawCounter);  // Display the current number and increment it
    tft.flush();

    // #ifdef !DEBUG_LOG
    if (touched) {
      touched = false;
      delay(10);
      tft.begin();
      tft.fillScreen(ILI9341_BLACK);
      tft.flush();
      delay(100);
    }
    // #endif
  }
  screenRedrawCounter++;
  // Reset the number to prevent overflow and keep it readable
  if (screenRedrawCounter > 9999) screenRedrawCounter = 0;
}

void TouchScreenTFT::loop() {
  // Check for touch events
  checkTouch();
  updateScreen();
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

  //Serial.println();
  if (p.x > 0 || p.y > 0) {
    touched = true;
    Serial.print("ax:");
    Serial.print(p.x);
    Serial.print(" ay:");
    Serial.print(p.y);
    Serial.print("     ");
  }


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
        return 4;
      }
    }
  }
    //delay(100);
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
