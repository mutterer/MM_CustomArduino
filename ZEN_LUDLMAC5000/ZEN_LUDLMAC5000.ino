

String cmd = "" ;
int done;


void setup() {
  Serial.begin( 9600, SERIAL_8N2 );
  done=0;
}


void loop()
{
  if (Serial.available()) {
    cmd = Serial.readStringUntil(0x0d);
    delay(10);

    processCommand(cmd);
    cmd = "";
  }
  /*
   *
   if ((millis()>2000)&&(done==0)) {
    reply ("Version no. : 6.300\n");
    done = 1;
  }
  */
}

void processCommand(String s) {
  if (s.indexOf("VER")>=0) {
    reply ("Version no. : 16.300\n\r");
  } else if (s.startsWith("!autostatus 0")) {
    delay(5);
  } 
}

void reply(String s) {
  Serial.print(s);
  delay(10);
  Serial.print(":A\n");
  delay(10);
  Serial.flush();
}
