#define IR_USE_AVR_TIMER1
#include <IRremote.hpp> 
#include <Wire.h>
#include "RTClib.h" 

/*
Librarys:
https://github.com/Arduino-IRremote/Arduino-IRremote
https://www.arduino.cc/reference/en/libraries/rtclib-by-neiron/
*/

/*
Components:
Modul ceas: https://ardushop.ro/ro/home/959-modul-ceas-in-timp-real-pcf8563.html
Detector PIR de miscare: https://ardushop.ro/ro/electronica/45-modul-pir-senzor-de-prezenta-miscare.html
Senzor IR telecomanda: https://www.optimusdigital.ro/en/others/755-modul-receptor-telecomanda-infrarou.html
Buzzer: https://www.ardumotive.com/how-to-use-a-buzzer-en.html
Shift Registers: https://ardushop.ro/ro/electronica/141-ic-shift-register-sn74hc595n-74hc595.html
Transistor pentru bec: C9013
Display: ...
*/

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

bool readSecs=false;
bool readHours=false;

bool lightState=false;

int BR_CHG_RATE=100;

int RECV_PIN = 11;
int LIGHT_PIN=2;
int count=0;

bool ALARM_RUNNING=false;
int BUZZER_PIN=7;

PCF8563 rtc;

int PIR_PIN=6;

bool disabled=true; //true
bool IR_REMOTE=false; //false;

bool disabledSmall=true;

void setup() {                
  Serial.begin(9600);
  Serial.println(F("\n------------------------------"));
  Serial.println(F("ClockINO Kernel launched!"));

  pinMode(LIGHT_PIN, OUTPUT);

  initDisplay();
  setDisplayBrightness(3000);

  initIr();
  //initBuzzer();
  initPir();
  initTime();

  initClockINORegisters();

  Serial<<F("[Alarm] Current time: ")<<rtc.now().year()<<F(" ")<<rtc.now().month()<<F(" ")<<rtc.now().day()<<F(" ")<<rtc.now().hour()<<F(" ")<<rtc.now().minute()<<F("\n");
  Serial<<F("[Alarm] Alarm time: ")<<rtc.get_alarm().year()<<F(" ")<<rtc.get_alarm().month()<<F(" ")<<rtc.get_alarm().day()<<F(" ")<<rtc.get_alarm().hour()<<F(" ")<<rtc.get_alarm().minute()<<F("\n");
      

}
void loop() {
  display();
  checkPir();
  checkIr();
  disableTimeReading();
  checkMemory();
  alarm();
}