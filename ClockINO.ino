#include <IRremote.hpp>
#define ul unsigned long

//TODO: Fix the column(:) dissappearing on some time (1:07)
//TODO: Make a new INO file (TimeHelper) with all functions related to time.

int RECV_PIN = A3;
int count=0;

ul prevMili, prevSec, prevMin;

void setup() {                

  initDisplay();

  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN);

  prevMili=millis();
  prevSec=prevMili/1000;
  prevMin=prevSec/60;

}
int irToNumber(String hexCode)
{
  String codeArray[10]={"ef10fb04", "ee11fb04", "ed12fb04", "ec13fb04", "eb14fb04", "ea15fb04", "e916fb04", "e817fb04", "e718fb04", "e619fb04"};
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
ul timeInMins=0;
void displayTime()
{
  int mins=timeInMins%60;
  int hours=timeInMins/60;
  if (hours==24) {
    timeInMins=0;
    displayTime();
  }
  int h1=hours/10, h2=hours%10, m1=mins/10, m2=mins%10;
  if (h1==1||h1==2) displayNumber(0, h1); 
  displayNumber(1, h2); displayNumber(2, m1); displayNumber(3, m2);

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

bool disabled=false, alreadyDisabled=false;


void loop() {
  setDisplayBrightness(3);
  if (readTime==false&&disabled==false&&alreadyDisabled==false) {
    displayTime();
  }

  if (disabled==true&&alreadyDisabled==false) {
    disableDigits();
    alreadyDisabled=true;
  }
  if (disabled==false&&alreadyDisabled==true) {
    alreadyDisabled=false;
  }

  if (readTime==true&&disabled==false&&alreadyDisabled==false) {
    if (digit1==1||digit1==2) displayNumber(0, digit1); 
    displayNumber(1, digit2); displayNumber(2, digit3); displayNumber(3, digit4);
  }

  if (IrReceiver.decode()) {
      unsigned long rawData=IrReceiver.decodedIRData.decodedRawData;
      IrReceiver.resume(); // Enable receiving of the next value
      if (rawData==0) return;
      String hexCode=String(rawData, HEX);
      if (hexCode=="f40bfb04") {
        readTime=true;
        digit1=0; digit2=0; digit3=0; digit4=0;
        poz=0;
        Serial.println("OK for Input");
        blink(10, 100);
        return;
      }

      if (hexCode=="f708fb04") {
        disabled=!disabled;
        return;
      }

      if (readTime==true) {
        int cif=irToNumber(hexCode);
        setTime(cif);
      }
      
  }

  if (poz==4) {
    readTime=false;
    int hours=digit1*10+digit2;
    int mins=digit3*10+digit4;
    timeInMins=hours*60+mins;
    digit1=0; digit2=0; digit3=0; digit4=0;
    poz=0;
  }


  if (readTime==false) {
    ul mili=millis();
    ul seconds=mili/1000;
    ul mins=seconds/60;
    if (seconds!=prevSec) {
      timeInMins++;
    }
    prevMili=mili;
    prevSec=seconds;
    prevMin=mins;
  }
  
}