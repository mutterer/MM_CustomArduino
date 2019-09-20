// An arduino firmware that can be used with the Micro-Manager Vincent D1 shutter device adaptor.
// version 0.1
// JM 
// References
// https://www.micro-manager.org/wiki/Vincent

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            3
#define NUMPIXELS      8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


byte cmd ;
int a = 100;     // a could be linked to a potentiometer on the device.
boolean state = false;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(20); // no line ending so rely on timeout
  pixels.begin();
}

void loop()
{
  if (Serial.available()) {
    cmd = Serial.read(); // no line ending so rely on timeout
    processCommand(cmd);
  }
}

// only open, close and reset are implemented here
void processCommand(byte s) {
  if (s==0x80) {
   state = true;
  } else if (s==0x81) {
       state = false;

  } else {
       state = false;
  } 
  // update pixels according to state and intensity
  // MM sends a [0..100] value, map this to [0..255]
  for (int i = 0; i < NUMPIXELS; i++) {
    double f = 2.55*state; 
    // a could be linked to a potentiometer on the device.
    pixels.setPixelColor(i, pixels.Color(f*a, f*a, f*a));
  }
  pixels.show();
}

