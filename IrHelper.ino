void initIr() {
  IrReceiver.begin(RECV_PIN);
  Serial.print(F("[OK] Launching Remote IR Sensor on pin: "));
  Serial.println(RECV_PIN);
}
#define zero 2774139135
#define one 2991390975
#define two 2974679295
#define three 2957967615
#define four 2924544255
#define five 2907832575
#define six 2891120895
#define seven 2857697535
#define eight 2840985855
#define nine 2824274175
int irToNumber(ul rawData)
{
  int cif=-1;
  switch (rawData) {
    case zero:
      cif=0; break;
    case one:
      cif=1; break;
    case two:
      cif=2; break;
    case three:
      cif=3; break;
    case four:
      cif=4; break;
    case five:
      cif=5; break;
    case six:
      cif=6; break;
    case seven:
      cif=7; break;
    case eight:
      cif=8; break;
    case nine:
      cif=9; break;
  }
  Serial.print(F("[OK] IRremote read cif: "));
  Serial.println(cif);
  return cif;
}
#define menu 2790850815
#define audio 3843686655
#define exit 2757427455
#define standby 3058237695
#define mute 3024814335
#define info 2690580735
#define volminus 3776839935
#define volplus 2740715775
#define fav 2807562495
#define chminus 3826974975
#define chplus 2707292415
#define ok 3760128255
#define recall 3910533375
void decodeIr(ul rawData)
{
  switch (rawData) {
    case menu:
      readTime=true;
      digit1=0; digit2=0; digit3=0; digit4=0;
      poz=0;
      Serial.println(F("[OK] Ready for Clock Input!"));
      blink(5, 200);
      break;
    case audio:
      digit1=0; digit2=0; digit3=0; digit4=0;
      readAlarm=true;
      poz=0;
      Serial.println(F("[OK] Ready for Alarm Input!"));
      blink(5, 200);
      break;
    case exit:
      disableAlarm();
      break;
    case standby:
      IR_REMOTE=!IR_REMOTE;
      delay(20);
      disabled=!disabled;
      //disableDigits();
      break;
    case mute:
      disabledSmall=!disabledSmall; break;
    case info:
      readSecs=!readSecs; break;
    case volminus:
      decreaseSeconds(); break;
    case volplus:
      increaseSeconds(); break;
    case fav:
      readHours=!readHours; break;
    case chminus:
      decreaseHours(); break;
    case chplus:
      increaseHours(); break;
    case ok:
      lightState=!lightState;
      digitalWriteFast(LIGHT_PIN, lightState);
      break;
    case recall:
      ALARM_RUNNING=!ALARM_RUNNING; break;
  }
  if (readTime==true||readAlarm==true) {
      int cif=irToNumber(rawData);
      setTime(cif);
  }
}
void checkIr()
{
  if (IrReceiver.decode()) {
      ul rawData=IrReceiver.decodedIRData.decodedRawData;
      IrReceiver.resume(); // Enable receiving of the next value
      if (rawData==0) return;
      Serial.print(F("[OK] Remote IR Raw Data: ")); Serial.println(rawData);
      decodeIr(rawData);
  }
} 
