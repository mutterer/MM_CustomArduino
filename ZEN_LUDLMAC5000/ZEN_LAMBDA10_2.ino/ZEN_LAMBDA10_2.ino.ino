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
  Serial.setTimeout(1000); // no line ending so rely on timeout
  pixels.begin();
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);

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
  if (s==0xee) {
   Serial.write(s);
   Serial.write(0x0d);
  } else if (s==0xfd) {
    // send out the id token and config string
   Serial.write(s);
   //Serial.print("10-3WA-NCWB-NCWC-NCSA-VSSB-VS");
   Serial.print("10-3WA-BDWB-NCWC-NCSA-VSSB-VS");
   Serial.write(0x0d);
   Serial.flush();
  } else if (s==0xcc) {
   // echo
   Serial.write(s);
   // FWA
   Serial.write(0x00);
   // FWB
   Serial.write(0x80);
   // FWC
   Serial.write(0xfc);
   Serial.write(0x00);
   // SA
   Serial.write(0xac);
   // SB
   Serial.write(0xbc);
   // SA mode
   Serial.write(0xdc);
   Serial.write(0x01);
   // SB mode
   Serial.write(0xdc);
   Serial.write(0x02);
   // confirmation
   Serial.write(0x0d);
   Serial.flush();
   
  } else if (s==0xaa) {
   Serial.write(s);
   Serial.write(0x0d);
   digitalWrite(13,HIGH);
  } else if (s==0xac) {
   Serial.write(s);
   Serial.write(0x0d);
   digitalWrite(13,LOW);
  } else {
   Serial.write(s);
   Serial.write(0x0d);
  } 
  
}
