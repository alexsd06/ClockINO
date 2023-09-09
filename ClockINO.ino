#include <IRremote.hpp>
#define ul unsigned long

//TODO: Fix the column(:) dissappearing on some time (1:07)
//TODO: Make a new INO file (TimeHelper) with all functions related to time.

int RECV_PIN = 11;
int count=0;

ul prevMili, prevSec, prevMin;

void setup() {                

  initDisplay();
  initDisplayPins_small();

  setDisplayBrightness(500);
  setDisplayBrightness_small(500);

  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN);

  prevMili=millis();
  prevSec=prevMili/1000;
  prevMin=prevSec/60;

}
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

bool readTime=false;
int poz=0;
int digit1=9, digit2=9, digit3=9, digit4=9;
ul timeInSecs=0;

bool disabled=false;
bool disabledSmall=true;

bool readSecs=false;
bool readBrightness=false;

void displayTime()
{
  ul secs=timeInSecs%60;
  ul mins=(timeInSecs/60)%60;
  ul timeInMins=timeInSecs/60;
  ul hours=timeInMins/60;
  if (hours==24) {
    timeInSecs=0;
    displayTime();
  }
  int h1=hours/10, h2=hours%10, m1=mins/10, m2=mins%10;
  int milli_current=millis()%100;
  int s1=secs/10, s2=secs%10;
  //int mili1=milli_current/10, mili2=milli_current%10;
  if (h1==1||h1==2) displayNumber(0, h1); 
  displayNumber(1, h2); displayNumber(2, m1); displayNumber(3, m2);
  if (disabledSmall==false) {
    displayNumber_small(0, s1); 
    displayNumber_small(1, s2); 
  }
  if (readSecs==true) showLowerLeftDot();
  if (readBrightness==true) showUpperLeftDot();
  //displayNumber_small(2, mili1); displayNumber_small(3, mili2);

}

void setTime(int cif) {
  if (readTime==true&&cif!=-1) {
    if (poz==0&&cif>=0&&cif<=2) {
      digit1=cif;
      poz++;
    }
    else if (poz==1) {
      if (digit1==2) {
        if(cif>=0&&cif<=3) {
          digit2=cif;
          poz++;
        }
      }
      else {
        digit2=cif;
        poz++;
      }
    }
    else if (poz==2) {
      if (cif>=0&&cif<=5) {
        digit3=cif;
        poz++;
      }
    }
    else if (poz==3) {
      digit4=cif;
      poz++;
    }
  }
}

int BR_CHG_RATE=100;

void loop() {
  if (readTime==false&&disabled==false) {
    displayTime();
  }

  if (readTime==true&&disabled==false) {
    if (digit1==1||digit1==2) displayNumber(0, digit1); 
    displayNumber(1, digit2); displayNumber(2, digit3); displayNumber(3, digit4);
  }

  if (IrReceiver.decode()) {
      unsigned long rawData=IrReceiver.decodedIRData.decodedRawData;
      IrReceiver.resume(); // Enable receiving of the next value
      if (rawData==0) return;
      String hexCode=String(rawData, HEX);
      Serial.println(hexCode);
      if (hexCode=="a65900ff") {  //Menu
        readTime=true;
        digit1=0; digit2=0; digit3=0; digit4=0;
        poz=0;
        Serial.println("OK for Input");
        blink(10, 100);
        disableDigits_small();
      }

      if (hexCode=="b64900ff") { //Standby
        disabled=!disabled;
        disabledSmall=!disabledSmall;
        disableDigits();
      }

      if (hexCode=="b44b00ff") { //Mute
        disabledSmall=!disabledSmall;
      }

      if (hexCode=="a05f00ff") {readSecs=!readSecs;} //Info
      if (hexCode=="e11e00ff"&&readSecs==true) timeInSecs--; //Vol--
      if (hexCode=="a35c00ff"&&readSecs==true) timeInSecs++; //Vol++

      if (hexCode=="a75800ff") readBrightness=!readBrightness;//Fav
      if (hexCode=="e41b00ff"&&readBrightness==true) { //Ch--
        if (getDisplayBrightness()-BR_CHG_RATE>=0) setDisplayBrightness(getDisplayBrightness()-BR_CHG_RATE);
        if (getDisplayBrightness_small()-BR_CHG_RATE>=0) setDisplayBrightness_small(getDisplayBrightness_small()-BR_CHG_RATE);
      } 
      if (hexCode=="a15e00ff"&&readBrightness==true) { //Ch++
        setDisplayBrightness(getDisplayBrightness()+BR_CHG_RATE);
        setDisplayBrightness_small(getDisplayBrightness_small()+BR_CHG_RATE);
      }

      if (readTime==true) {
        int cif=irToNumber(hexCode);
        setTime(cif);
      }

      
  }

  if (poz==4) {
    readTime=false;
    ul hours=digit1*10+digit2;
    ul mins=digit3*10+digit4;
    timeInSecs=(hours*3600)+(mins*60);
    digit1=0; digit2=0; digit3=0; digit4=0;
    poz=0;
  }

  ul micro=micros();
  ul mili=micro/1000;
  ul seconds=mili/1000;
  ul mins=seconds/60;
  if (readTime==false) {
    if (seconds!=prevSec) {
      timeInSecs++;
    }
  }
  prevMili=mili;
  prevSec=seconds;
  prevMin=mins;
}