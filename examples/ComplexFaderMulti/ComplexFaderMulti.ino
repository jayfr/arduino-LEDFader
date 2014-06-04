/******************************************

Example of modified LEDFader library in conjunction with
Adafruit's TLC59711 and hard wired LEDs.

******************************************/

#include <LEDFader.h>
#include "Adafruit_TLC59711.h"
#include <SPI.h>

// How many boards do you have chained?
#define NUM_TLC59711 1

#define spiData   11
#define spiClock  13

Adafruit_TLC59711 tlc = Adafruit_TLC59711(NUM_TLC59711, spiClock, spiData);

// An array of 6 LEDs controlled by LEDFader
// Three are hard connected to PWM pins 3, 5, and 6
// Three are soft connected using pwmHelper, on R0, G0, and B0

#define TotalLeds 6

LEDFader led[TotalLeds] = {
  LEDFader(PWM_PIN, 3),
  LEDFader(PWM_PIN, 5),
  LEDFader(PWM_PIN, 6),
  LEDFader(pwmHelper, 0),
  LEDFader(pwmHelper, 1),
  LEDFader(pwmHelper, 2)
};

int ledValue[TotalLeds] = {0, 0, 0, 0, 0, 0};
int ledTime[TotalLeds] = {500, 1000, 1500, 2000, 2500, 3000};

// pwmHelper must be declared as void, with two uint8_t parameters
void pwmHelper(uint8_t channel, uint8_t value) {
  // LEDFader utilizes 8 bit pwm values
  // TLC59711 utilizes 16 bit pwm values
  // Convert the 8-bit to 16-bit and shift left 8 bits
  uint16_t bigValue = ((uint16_t)value) << 8;
  tlc.setPWM(channel, bigValue); 
}


void setup() {
  // need to initialize the TLC59711
  tlc.begin();
  tlc.write();
}


void loop() {  


  // loop through the LEDs
  for (int i = 0; i < TotalLeds; i++) {
    
    // if the LEDs aren't fading, start them fading in the opposite direction

    if (!led[i].is_fading()) {
      if (ledValue[i] == 0)
        ledValue[i] = 255;
      else
        ledValue[i] = 0;
        
      led[i].fade(ledValue[i], ledTime[i]);
    }  

    // tell LEDFader to update the pwm values
    // hardwired LEDs will be automatically updated here
    // softwired LEDs will use the helper function
    led[i].update();

  }
  
  // our soft wired LEDs still need their own updating
  // with this hardware, all the soft LEDs are update at once
  tlc.write();

  // loop delay
  delay(50);
}

