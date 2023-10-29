void initIr() {IrReceiver.begin(RECV_PIN);}

int irToNumber(String hexCode)
{
  //////////////////////    0            1           2          3           4           5           6           7           8            9
  String codeArray[10]={"a55a00ff", "b24d00ff", "b14e00ff", "b04f00ff", "ae5100ff", "ad5200ff", "ac5300ff", "aa5500ff", "a95600ff", "a85700ff"};
  Serial.println(hexCode);
  for (int i=0; i<10; i++) {
    if (hexCode==codeArray[i])
      return i;
  }
  return -1;
}

void decodeIr(String hexCode)
{
  if (hexCode=="a65900ff") {  //Menu
      readTime=true;
      digit1=0; digit2=0; digit3=0; digit4=0;
      poz=0;
      Serial.println("OK for Input");
      blink(10, 100);
      disableDigits_small();
    }
    else if (hexCode=="e51a00ff") { //Audio
      digit1=0; digit2=0; digit3=0; digit4=0;
      readAlarm=true;
      digit1=0; digit2=0; digit3=0; digit4=0;
      poz=0;
      Serial.println("OK for Input");
      blink(5, 100);
      disableDigits_small();
    }
    else if (hexCode=="a45b00ff") { //Exit
      disableAlarm();
      Serial<<"Now: "<<rtc.now().year()<<" "<<rtc.now().month()<<" "<<rtc.now().day()<<" "<<rtc.now().hour()<<" "<<rtc.now().minute()<<'\n';
      Serial<<"Alarm: "<<rtc.get_alarm().year()<<" "<<rtc.get_alarm().month()<<" "<<rtc.get_alarm().day()<<" "<<rtc.get_alarm().hour()<<" "<<rtc.get_alarm().minute()<<'\n';
      
    }
    else if (hexCode=="b64900ff") { //Standby
      IR_REMOTE=!IR_REMOTE;
      delay(20);
      disabled=!disabled;
      //disabledSmall=!disabledSmall;
      disableDigits();
    }

    else if (hexCode=="b44b00ff") { //Mute
      disabledSmall=!disabledSmall;
    }

    else if (hexCode=="a05f00ff") readSecs=!readSecs; //Info
    else if (hexCode=="e11e00ff"&&readSecs==true) decreaseSeconds(); //Vol --
    else if (hexCode=="a35c00ff"&&readSecs==true) increaseSeconds(); //Vol ++
    /*
    //else if (hexCode=="a75800ff") readBrightness=!readBrightness;//Fav
    else if (hexCode=="e41b00ff"&&readBrightness==true) { //Ch--
      if (getDisplayBrightness()-BR_CHG_RATE>=0) setDisplayBrightness(getDisplayBrightness()-BR_CHG_RATE);
      if (getDisplayBrightness_small()-BR_CHG_RATE>=0) setDisplayBrightness_small(getDisplayBrightness_small()-BR_CHG_RATE);
    }
    else if (hexCode=="a15e00ff"&&readBrightness==true) { //Ch++
      setDisplayBrightness(getDisplayBrightness()+BR_CHG_RATE);
      setDisplayBrightness_small(getDisplayBrightness_small()+BR_CHG_RATE);
    }
    */
    else if (hexCode=="a75800ff") readHours=!readHours;//Fav
    else if (hexCode=="e41b00ff"&&readHours==true) decreaseHours(); //Ch--
    else if (hexCode=="a15e00ff"&&readHours==true) increaseHours(); //Ch++
    else if (hexCode=="e01f00ff") { //OK
      lightState=!lightState;
      digitalWriteFast(LIGHT_PIN, lightState);
    }

    else if (hexCode=="e91600ff") {
      ALARM_RUNNING=!ALARM_RUNNING;
    }

    else if (readTime==true||readAlarm==true) {
      int cif=irToNumber(hexCode);
      setTime(cif);
    }
}
void checkIr()
{
  if (IrReceiver.decode()) {
      unsigned long rawData=IrReceiver.decodedIRData.decodedRawData;
      IrReceiver.resume(); // Enable receiving of the next value
      if (rawData==0) return;
      String hexCode=String(rawData, HEX);
      Serial.println(hexCode);
      decodeIr(hexCode);
  }
} 
