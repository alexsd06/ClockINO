#include <IRremote.hpp>

int RECV_PIN = 11;
int count=0;
void setup() {                

  initDisplayPins();

  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN);

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

// the loop routine runs over  and over again forever:
bool readTime=false;
int poz=0;
int digit1=9, digit2=9, digit3=9, digit4=9;
void loop() {
  //increment(); 
  displayNumber(0, digit1); displayNumber(1, digit2); displayNumber(2, digit3); displayNumber(3, digit4);
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
      }

      if (poz==5) readTime=false;

      int cif=irToNumber(hexCode);
      if (cif!=-1&&readTime==true) {
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
  
}