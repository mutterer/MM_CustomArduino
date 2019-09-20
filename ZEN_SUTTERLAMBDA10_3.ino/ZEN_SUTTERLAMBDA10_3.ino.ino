// An arduino firmware that emulates some functions of a SUTTER LAMBDA 10-3 controller.
// version 0.1
// JM 
// References
// https://www.sutter.com/manuals/LB10-3_OpMan.pdf

byte cmd ;

void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop()
{
  if (Serial.available()) {
    cmd = Serial.read(); 
    processCommand(cmd);
  }
}

void processCommand(byte s) {
  if (s==0xee) {
   Serial.write(s); // echo ONLINE cmd
   Serial.write(0x0d);
  }  else if (s==0xfd) {
   Serial.write(s); // echo CONFIG cmd
   Serial.print("10-3WA-BDWB-NCWC-NCSA-VSSB-VS");
   Serial.write(0x0d);
   Serial.flush();
  } else if (s==0xcc) {
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
   
  } else if (s==0xaa) {
   Serial.write(s);      // echo SA OPEN
   Serial.write(0x0d);
   digitalWrite(13,HIGH);
  } else if (s==0xac) {
   Serial.write(s);      // echo SA CLOSE
   Serial.write(0x0d); 
   digitalWrite(13,LOW);
  } else {
   Serial.write(s); // echo any other case
   Serial.write(0x0d);
  } 
  
}
