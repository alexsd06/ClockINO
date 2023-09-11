#include <IRremote.hpp>
#define ul unsigned long

bool readTime=false;
int poz=0;
int digit1=9, digit2=9, digit3=9, digit4=9;
ul timeInSecs=0;

bool disabled=false;
bool disabledSmall=true;

bool readSecs=false;
bool readBrightness=false;

bool lightState=false;

int BR_CHG_RATE=100;

int RECV_PIN = 11;
int LIGHT_PIN=8;
int count=0;

ul prevMili, prevSec, prevMin;

void setup() {                

  pinMode(LIGHT_PIN, OUTPUT);

  initDisplay();
  initDisplayPins_small();

  setDisplayBrightness(500);
  setDisplayBrightness_small(500);

  initIr();
  Serial.begin(9600);
  
  initTime();

}
void loop() {
  display();
  checkIr();
  disableTimeReading();
  increaseTime();
}