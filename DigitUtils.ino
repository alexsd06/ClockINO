#include <ShiftRegister74HC595.h>

int turnedOn[28]; //maxim 14 + turnedOn[0] care retine dimensiunea

int DISPLAY_BRIGHTNESS;
int controls[]={39, 40};
////////////////0   1   2   3   4   5   6   7   8   9   10  11  12  13
int segments[]={24, 25, 26, 27, 28, 30, 31, 32, 33, 34, 35, 36, 37, 38};

String segmentCodes[4][7]={ ///codes[digit][segment(as index of segments)]="activation code";
  // A      B      C      D      E      F      GA 
  {"1-1", "0-0", "0-3", "1-2", "0-2", "!-!", "0-1" }, //digit 0
  {"0-6", "0-4", "0-5", "1-5", "1-3", "1-6", "1-4" }, //digit 1
  {"1-7", "1-8", "1-9", "0-9", "0-10","0-7", "0-8" }, //digit 2
  {"0-13","0-11","0-12","1-12","1-10","1-13","1-11"}  //digit 4 
};
String digitCodes[10]={
  //0        1       2        3       4        5        6        7        8         9
  "ABCDEF", "BC", "ABGED", "ABGCD", "FGBC", "AFGCD", "AFEDGC", "ABC", "ABCDEFG", "GFABCD"
};
int column=41;
int lower_left_dot=22;
int upper_left_dot=23;
//int something = 29; Probably broken lower right dot.

void initDisplay()
{
  for (int i=0; i<sizeof(controls)/sizeof(int); i++) pinMode(controls[i], OUTPUT);
  for (int i=0; i<sizeof(segments)/sizeof(int); i++) {
    if (segments[i]>=39||segments[i]==30||segments[i]==31) pinMode(segments[i], OUTPUT);
  }
  for (int i=0; i<sizeof(turnedOn)/sizeof(int); i++) turnedOn[i]=0;
  pinMode(column, OUTPUT);
  pinMode(lower_left_dot, OUTPUT);
  pinMode(upper_left_dot, OUTPUT);
  //pinMode(something, OUTPUT);
}

void setDisplayBrightness(int bright) {DISPLAY_BRIGHTNESS=bright;}

int getDisplayBrightness() {return DISPLAY_BRIGHTNESS;}

void enableControl0() {
  digitalWrite(controls[1], HIGH);
  digitalWrite(controls[0], LOW);
}
void enableControl1() {
  digitalWrite(controls[0], HIGH);
  digitalWrite(controls[1], LOW);
}

void disableControl0() {digitalWrite(controls[0], HIGH);}
void disableControl1() {digitalWrite(controls[1], HIGH);}

const int numberOfShiftRegisters = 2; // number of shift registers attached in series
const int serialDataPin = 26; // DS
const int clockPin = 23; // SHCP
const int latchPin = 24; // STCP
ShiftRegister74HC595<numberOfShiftRegisters> sr(serialDataPin, clockPin, latchPin);

int getRealPin(int pin) {
   if (pin<30) {
    int endPin=29-pin;
    return endPin;
   } else if (pin>31&&pin<39) {
    int endPin=39-pin+7+1;
    return endPin;
   } else {
    return pin;
   }
}

void turnOnSegmentPin(int pin) {
  if (pin<30) {
    int endPin=29-pin;
    sr.set(endPin, HIGH);
    //Serial.println("HERE");
  } else if (pin>31&&pin<39) {
    int endPin=39-pin+7+1;
    sr.set(endPin, HIGH);
    //Serial.println(endPin);
  } else {
    digitalWrite(pin, HIGH);
  }
  turnedOn[++turnedOn[0]]=pin;
}
void turnOffSegmentPin(int pin) {
  if (pin<30) {
    int endPin=29-pin;
    sr.set(endPin, LOW);
  } else if (pin>31&&pin<39) {
    int endPin=39-pin+7+1;
    sr.set(endPin, LOW);
  } else {
    digitalWrite(pin, LOW);
  }
}

void disableDigits()
{
  setDigits_small(0);
  hideColumn();
  hideLowerLeftDot();
  hideUpperLeftDot();
  //hideSomething();

  disableControl0();
  disableControl1();
  for (int i=1; i<=turnedOn[0]; i++) {
    turnOffSegmentPin(turnedOn[i]);
    turnedOn[i]=0;
  }
  turnedOn[0]=0;
}

void showColumn()
{
  disableDigits();
  enableControl1();
  digitalWrite(column, HIGH);
  delayMicroseconds(DISPLAY_BRIGHTNESS);
}
void hideColumn() {digitalWrite(column, LOW);}

int control0Pins[105];
int control1Pins[105];

void sortArray(int arr[])
{
  for (int i=1; i<arr[0]; i++) {
    for (int j=i+1; j<=arr[0]; j++) {
      if (arr[i]>arr[j]) {
        int c=arr[i]; arr[i]=arr[j]; arr[j]=c;
      }
    }
  }
}

void resetControlArrays()
{
  control0Pins[0]=0;
  control1Pins[0]=0;
}

void displayControlArrays()
{
  for (int i=1; i<=control0Pins[0]; i++) {
    control0Pins[i]=getRealPin(control0Pins[i]);
  } 
  for (int i=1; i<=control1Pins[0]; i++) {
    control1Pins[i]=getRealPin(control1Pins[i]);
  }
  //TODO: Send all pins on control 0 and 1 at once to the shift registers!
  sortArray(control0Pins); sortArray(control1Pins);
  disableDigits();
  enableControl0();
  for (int i=1; i<=control0Pins[0]; i++) {
    turnOnSegmentPin(control0Pins[i]);
  }
  delayMicroseconds(DISPLAY_BRIGHTNESS);
  disableControl0(); 
  disableDigits();
  enableControl1();
  for (int i=1; i<=control1Pins[0]; i++) {
    turnOnSegmentPin(control1Pins[i]);
  }
  delayMicroseconds(DISPLAY_BRIGHTNESS);
  disableControl1();
  resetControlArrays();
}

int turnOnSegment(int poz, char seg) {
  int segCode=(int)(seg-'A');
  String activationCode=segmentCodes[poz][segCode];
  //int hyphenIndex=activationCode.indexOf("-");
  int hyphenIndex=1; //it's always the second character
  if (hyphenIndex!=-1) {
    int control=activationCode.substring(0, hyphenIndex).toInt();
    int segmentArrayIndex=activationCode.substring(hyphenIndex+1).toInt();
    
    disableDigits();
    
    int pin=segments[segmentArrayIndex];
    if (control==0) {
      enableControl0();
      control0Pins[++control0Pins[0]]=pin;
    }
    if (control==1) {
      enableControl1();
      control1Pins[++control1Pins[0]]=pin;
    }
    turnOnSegmentPin(pin);
    delayMicroseconds(DISPLAY_BRIGHTNESS);
  }
}


int displayNumber (int poz, int nr) {
  String segmentsNeeded=digitCodes[nr];
  for (int i=0; i<segmentsNeeded.length(); i++) {
    char segment=segmentsNeeded[i];
    turnOnSegment(poz, segment);
  }
}

void showLowerLeftDot()
{
  disableDigits();
  enableControl1();
  digitalWrite(lower_left_dot, HIGH);
  delayMicroseconds(DISPLAY_BRIGHTNESS);
}
void hideLowerLeftDot() {digitalWrite(lower_left_dot, LOW);}

void showUpperLeftDot()
{
  disableDigits();
  enableControl1();
  digitalWrite(upper_left_dot, HIGH);
  delayMicroseconds(DISPLAY_BRIGHTNESS);
}
void hideUpperLeftDot() {digitalWrite(upper_left_dot, LOW);}
/*
void showSomething()
{
  enableControl1();
  digitalWrite(something, HIGH);
  delayMicroseconds(DISPLAY_BRIGHTNESS);
}
void hideSomething() {digitalWrite(something, LOW);}
*/
void blink (int repeat, int del)
{
   bool disabled=false;
   while (repeat!=0) {
    unsigned long m=millis();
    if (m%(del*2)>=0&&m%(del*2)<=del) {
      displayNumber(1, 0);
      displayNumber(2, 0);
      displayNumber(3, 0);
      showColumn();
      disabled=false;
    }
    else {
      if (disabled==false) {
        disableDigits();
        disabled=true;
        repeat--;
      }
    }
   }
}