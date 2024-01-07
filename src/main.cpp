#include <Arduino.h>
#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>

// put function declarations here:
int myFunction(int, int);

// For optimized ILI9341_t3 library
#define TFT_DC      9
#define TFT_CS      10
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI    11
#define TFT_SCLK    13  // Cant change this
#define TFT_MISO    12 
#define TOUCH_CS    8   // Chip select pin for the touchscreen

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
XPT2046_Touchscreen touch(TOUCH_CS);


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

// Function declarations
void button1Function();
void button2Function();
void button3Function();
void button4Function();


void setup() {
  
  Serial.begin(9600);
  tft.begin();
  touch.begin();
  tft.setRotation(3);

  tft.fillScreen(ILI9341_BLACK);
  tft.fillRect(BUTTON_1_RED_X, BUTTON_1_RED_Y, BUTTON_W, BUTTON_H, ILI9341_RED);
  tft.fillRect(BUTTON_2_GREEN_X, BUTTON_2_GREEN_Y, BUTTON_W, BUTTON_H, ILI9341_GREEN);
  tft.fillRect(BUTTON_3_BLUE_X, BUTTON_3_BLUE_Y, BUTTON_W, BUTTON_H, ILI9341_BLUE);
  tft.fillRect(BUTTON_4_YELLOW_X, BUTTON_4_YELLOW_Y, BUTTON_W, BUTTON_H, ILI9341_YELLOW);
}

void loop() {

// Example raw touch values (minimum and maximum)
int touchXmin = 300;  // Replace with your minimum x value
int touchXmax = 3834; // Replace with your maximum x value
int touchYmin = 240;  // Replace with your minimum y value
int touchYmax = 3722; // Replace with your maximum y value
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
      }
    }
    if (p.x >= BUTTON_2_GREEN_X && p.x < (BUTTON_2_GREEN_X + BUTTON_W)) {
      if (p.y >= BUTTON_2_GREEN_Y && p.y < (BUTTON_2_GREEN_Y + BUTTON_H)) {
        button2Function();
      }
    }
    if (p.x >= BUTTON_3_BLUE_X && p.x < (BUTTON_3_BLUE_X + BUTTON_W)) {
      if (p.y >= BUTTON_3_BLUE_Y && p.y < (BUTTON_3_BLUE_Y + BUTTON_H)) {
        button3Function();
      }
    }
    if (p.x >= BUTTON_4_YELLOW_X && p.x < (BUTTON_4_YELLOW_X + BUTTON_W)) {
      if (p.y >= BUTTON_4_YELLOW_Y && p.y < (BUTTON_4_YELLOW_Y + BUTTON_H)) {
        button4Function();
      }
    }

    delay(10);
  }

   // wait for a second
  // delay(1000);

  // int result = myFunction(2, 3);
  // Serial.print(result);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
void button1Function() {
  Serial.println();
  Serial.print("button 1 red");
  Serial.println();
}

void button2Function() {
  Serial.println();
  Serial.print("button 2 green");
  Serial.println();
}

void button3Function() {
  Serial.println();
  Serial.print("button 3 blue");
  Serial.println();
}

void button4Function() {
  Serial.println();
  Serial.print("button 4 yellow");
  Serial.println();
}