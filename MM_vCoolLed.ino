// An arduino firmware that can be used with the Micro-Manager CoolLED device adaptor.
// version 0.1
// JM 2016-2018
// References
// https://www.micro-manager.org/wiki/PrecisExcite

#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            3
#define NUMPIXELS      8
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


String cmd = "" ;
int a = 0;
int b = 0;
int c = 0;
boolean state = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("MiFoBio 2018");
  lcd.setCursor(0, 1);
  lcd.print("vCoolLED ready");
  delay(1000);
  reply("Greetings"); // MM reads potential greetings form device.
  pixels.begin();
}

void loop()
{
  if (Serial.available()) {
    // every command sent from MM ends with a carriage return
    cmd = Serial.readStringUntil('\r');
    processCommand(cmd);
    cmd = "";
  }
}

// this method processes the recieved command
void processCommand(String s) {
  if (s.startsWith("C?")) {
    // general state query
    if (state == true ) {
      reply ("LAMAON");
      reply ("LAMBON");
      reply ("LAMCON");
    } else {
      reply ("LAMAOFF");
      reply ("LAMBOFF");
      reply ("LAMCOFF");
    }
  } else if (s.startsWith("LAMS")) {
    // query about installed channels
    reply ("LAM:A:Channel_A");
    reply ("LAM:B:Channel_B");
    reply ("LAM:C:Channel_C");
  } else if (s.startsWith("CA?")) {
    // query the current intensity for channel A
    String ia = String (a);
    reply (ia);
  } else if (s.startsWith("CB?")) {
    String ib = String (b);
    reply (ib);
  } else if (s.startsWith("CC?")) {
    String ic = String (c);
    reply (ic);  
  } else if (s.startsWith("CAI")) {
    // sets the insity for channel A
    String is = s.substring(s.indexOf("I") + 1);
    a = is.toInt();
    reply ("ok");
  } else if (s.startsWith("CBI")) {
    String is = s.substring(s.indexOf("I") + 1);
    b = is.toInt();
    reply ("ok");
  }  else if (s.startsWith("CCI")) {
    String is = s.substring(s.indexOf("I") + 1);
    c = is.toInt();
    reply ("ok");
  } else if (s.startsWith("SQX")) {
    // this switches the current state
    cmd = Serial.readStringUntil('\r');
    cmd = Serial.readStringUntil('\r');
    state = !state;
  } else if (s.startsWith("SQZ")) {
    cmd = Serial.readStringUntil('\r');
    state = !state;
  }
  // update state and channel info on lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lights ");
  if (state) {
    lcd.print("ON");
  } else {
    lcd.print("OFF");
  }
  lcd.setCursor(0, 1);
  lcd.print("A");
  lcd.print(a);
  lcd.setCursor(6, 1);
  lcd.print("B");
  lcd.print(b);
  lcd.setCursor(12, 1);
  lcd.print("C");
  lcd.print(c);
  
  // update pixels according to state and intensity
  // MM sends a [0..100] value, map this to [0..255]
  for (int i = 0; i < NUMPIXELS; i++) {
    double f = 2.55*state; 
    pixels.setPixelColor(i, pixels.Color(f*c, f*b, f*a));
  }
  pixels.show();
}

// a reply method that ends replies with a new line chr
void reply(String s) {
  Serial.print(s);
  Serial.print("\n");
}
