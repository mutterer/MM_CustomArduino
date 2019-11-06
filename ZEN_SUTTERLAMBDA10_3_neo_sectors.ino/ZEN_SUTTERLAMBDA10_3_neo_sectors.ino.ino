// An arduino firmware that emulates some functions of a SUTTER LAMBDA 10-3 controller.
// version 0.1
// JM
// References
// https://www.sutter.com/manuals/LB10-3_OpMan.pdf
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            3
#define NUMPIXELS      8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


byte cmd ;
int r, g, b;
int state;
int mode;
int mstart[] = {0, 0, 2, 4, 6, 0, 4};
int mstop[] = {7, 1, 3, 5, 7, 3, 7};

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(3000);
  pixels.begin();
  r = 100;
  g = 100;
  b = 100;
  state = 0;
  mode = 0;
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  updateLEDs();
}

void loop()
{
  if (Serial.available()) {
    cmd = Serial.read();
    processCommand(cmd);
  }
}

void processCommand(byte s) {
  if (s == 0xee) {
    Serial.write(s); // echo ONLINE cmd
    Serial.write(0x0d);
  }  else if (s == 0xfd) {
    Serial.write(s); // echo CONFIG cmd
    Serial.print("10-3WA-BDWB-NCWC-NCSA-VSSB-VS");
    Serial.write(0x0d);
    Serial.flush();
  } else if (s == 0xcc) {
    Serial.write(s);      // echo STATUS cmd
    Serial.write(0x00);   // FWA
    Serial.write(0x80);   // FWB
    Serial.write(0xfc);   // FWC
    Serial.write(0x00);
    Serial.write(0xac);   // SA closed
    Serial.write(0xbc);   // SB closed
    Serial.write(0xdc);   // SA SmartShutter Fast mode
    Serial.write(0x01);
    Serial.write(0xdc);   // SB SmartShutter Fast mode
    Serial.write(0x02);
    Serial.write(0x0d);   // confirmation
    Serial.flush();

  } else if (s == 0xaa) {
    Serial.write(s);      // echo SA OPEN
    Serial.write(0x0d);
    digitalWrite(13, HIGH);
    state = 1;
    updateLEDs();

  } else if (s == 0xac) {
    Serial.write(s);      // echo SA CLOSE
    Serial.write(0x0d);
    digitalWrite(13, LOW);
    state = 0;
    updateLEDs();

  } else {
    if (s == 0x10) {
      r = 100; g = 100; b = 100;
    } else if (s == 0x11) {
      r = 00; g = 00; b = 100;
    } else if (s == 0x12) {
      r = 00; g = 100; b = 00;
    } else if (s == 0x13) {
      r = 100; g = 00; b = 00;
    } else if (s == 0x14) {
      mode = (mode + 1) % 7;
    } else if (s == 0x19) {
      mode = 0;
    }
    Serial.write(s); // echo any other case
    Serial.write(0x0d);
    updateLEDs();

  }

}

void updateLEDs() {
  for (int i = 0; i < NUMPIXELS; i++) {
    double f = 2.55 * state; // could be linked to a potentiometer on the device.
    // 
    if ((i >= mstart[mode]) && (i <= mstop[mode])) {
      f = 2.55 * state;
    } else {
      f = 0;
    }
    pixels.setPixelColor(i, pixels.Color(f * r, f * g, f * b));
  }
  pixels.show();
}
