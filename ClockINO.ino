#define IR_USE_AVR_TIMER1
#include <IRremote.hpp> //https://github.com/Arduino-IRremote/Arduino-IRremote
#include <Wire.h>
#include "RTClib.h" //https://github.com/NeiroNx/RTCLib
//https://www.circuitbasics.com/how-to-setup-passive-infrared-pir-motion-sensors-on-the-arduino/
#define YMD rtc.now().year(), rtc.now().month(), rtc.now().day()
#define ul unsigned long

template <typename T>
Print& operator<<(Print& printer, T value)
{
    printer.print(value);
    return printer;
}


bool readTime=false;
bool readAlarm=false;
int poz=0;
int digit1=9, digit2=9, digit3=9, digit4=9;
//ul timeInSecs=0;

bool disabled=true; //true
bool disabledSmall=true;

bool readSecs=false;
//bool readBrightness=false;
bool readHours=false;

bool lightState=false;

int BR_CHG_RATE=100;

int RECV_PIN = 11;
int LIGHT_PIN=8;
int count=0;

bool ALARM_RUNNING=false;
int BUZZER_PIN=A8;

PCF8563 rtc;

int PIR_PIN=A0;
bool IR_REMOTE=false;

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
  initPir();
  initTime();

  initClockINORegisters();

  Serial<<"Now: "<<rtc.now().year()<<" "<<rtc.now().month()<<" "<<rtc.now().day()<<" "<<rtc.now().hour()<<" "<<rtc.now().minute()<<'\n';
  Serial<<"Alarm: "<<rtc.get_alarm().year()<<" "<<rtc.get_alarm().month()<<" "<<rtc.get_alarm().day()<<" "<<rtc.get_alarm().hour()<<" "<<rtc.get_alarm().minute()<<'\n';
      

}
void loop() {
  display();
  checkPir();
  checkIr();
  disableTimeReading();
  alarm();
}