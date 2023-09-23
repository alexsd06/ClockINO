#define IR_USE_AVR_TIMER1
#include <IRremote.hpp>
#include <Wire.h>
#include "RTClib.h"

#define YMD rtc.now().year(), rtc.now().month(), rtc.now().day()

#define ul unsigned long

bool readTime=false;
bool readAlarm=false;
int poz=0;
int digit1=9, digit2=9, digit3=9, digit4=9;
//ul timeInSecs=0;

bool disabled=false;
bool disabledSmall=true;

bool readSecs=false;
bool readBrightness=false;

bool lightState=false;

int BR_CHG_RATE=100;

int RECV_PIN = 11;
int LIGHT_PIN=8;
int count=0;

bool ALARM_RUNNING=false;
int BUZZER_PIN=A8;

PCF8563 rtc;

void setup() {                

  Serial.begin(9600);
  pinMode(13, OUTPUT); //Disable Onboard Pin 13 Light.
  digitalWrite(13, LOW);

  pinMode(LIGHT_PIN, OUTPUT);

  initDisplay();
  initDisplayPins_small();

  setDisplayBrightness(300);
  setDisplayBrightness_small(300);

  initIr();
  initBuzzer();
  initTime();

}
void loop() {
  display();
  checkIr();
  disableTimeReading();
  alarm();
}