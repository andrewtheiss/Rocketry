#include <Arduino.h>
#include <ILI9341_t3.h>

// put function declarations here:
int myFunction(int, int);

// For optimized ILI9341_t3 library
#define TFT_DC      9
#define TFT_CS      10
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI    11
#define TFT_SCLK    13
#define TFT_MISO    12
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);




void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // SPI.setMOSI(7);
  // SPI.setSCK(14);
  tft.begin();
  tft.fillScreen(tft.color565(0, 45, 255));
}

void loop() {
  // put your main code here, to run repeatedly:
    // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  tft.fillScreen(tft.color565(255, 45, 55));
  // wait for a second
  delay(5000);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
  tft.fillScreen(tft.color565(0, 5, 255));
   // wait for a second
  delay(1000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}