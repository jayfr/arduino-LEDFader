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

// One LED is hard wired to Digital 3 of the Arduino (through a resistor!)
LEDFader hardWiredLED = LEDFader(3);

// One LED is soft wired through channel 0 of the TLC59711
// We need to tell LEDFader to use the pwmHelper function
LEDFader softWiredLED = LEDFader(pwmHelper, 0);

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

int hardValue = 0;
int softValue = 0;

void loop() {  

  // if the LEDs aren't fading, start them fading in the opposite direction

  if (!hardWiredLED.is_fading()) {
    if (hardValue == 0)
      hardValue = 255;
    else
      hardValue = 0;
    
    // this one will fade over 1 second  
    hardWiredLED.fade(hardValue, 1000);
  }  

  if (!softWiredLED.is_fading()) {
    if (softValue == 0)
      softValue = 255;
    else
      softValue = 0;
    
    // this one will fade over 2 seconds  
    softWiredLED.fade(softValue, 2000);
  }  
  
  // tell LEDFader to update the pwm values
  // hardwired LEDs will be automatically updated here
  // softwired LEDs will use the helper function
  hardWiredLED.update();
  softWiredLED.update();

  // our soft wired LEDs still need their own updating
  tlc.write();

  // loop delay
  delay(50);
}

