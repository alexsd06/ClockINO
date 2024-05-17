//#include <ShiftRegister74HC595.h>

int turnedOn[20]; //maxim 14 + turnedOn[0] care retine dimensiunea

int DISPLAY_BRIGHTNESS;
int controls[]={12, 13};
////////////////0   1   2   3   4   5   6   7   8   9   10  11  12  13
int segments[]={24, 25, 26, 27, 28, 30, 31, 32, 33, 34, 35, 36, 37, 38};
const short int segmentCodes[4][7][2] = {
  {{1, 1}, {0, 0}, {0, 3}, {1, 2}, {0, 2}, {1, 1}, {0, 1}}, // digit 0
  {{0, 6}, {0, 4}, {0, 5}, {1, 5}, {1, 3}, {1, 6}, {1, 4}}, // digit 1
  {{1, 7}, {1, 8}, {1, 9}, {0, 9}, {0, 10}, {0, 7}, {0, 8}}, // digit 2
  {{0, 13}, {0, 11}, {0, 12}, {1, 12}, {1, 10}, {1, 13}, {1, 11}} // digit 4
};

const ul digitCodes[10] = {
  123456, 23, 12754, 12734, 6723, 16734, 165473, 123, 1234567, 761234
};
int column=5;

const int lower_left_dot=22;
bool lower_left_enabled=false;

const int upper_left_dot=23;
bool upper_left_enabled=false;

//int something = 29; Probably broken lower right dot.

void initDisplay()
{
  for (int i=0; i<sizeof(controls)/sizeof(int); i++) pinMode(controls[i], OUTPUT);
  pinMode(3, OUTPUT); pinMode(4, OUTPUT);
  for (int i=0; i<sizeof(turnedOn)/sizeof(int); i++) turnedOn[i]=0;
  pinMode(column, OUTPUT);
  pinMode(lower_left_dot, OUTPUT);
  pinMode(upper_left_dot, OUTPUT);
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

int getRealPin(int pin) {
   if (pin<30) {
    int endPin=29-pin;
    return endPin;
   } else if (pin>31&&pin<39) {
    int endPin=39-pin+7+1; //+1
    return endPin;
   } else {
    return pin;
   }
}

void disableDigits()
{
  hideColumn();
  disableControl0();
  disableControl1();
}

void showColumn()
{
  disableDigits();
  enableControl1();
  digitalWrite(column, HIGH);
  delayMicroseconds(DISPLAY_BRIGHTNESS);
}
void hideColumn() {digitalWrite(column, LOW);}

int control0Bits[21], real0Pins[6];
int control1Bits[21], real1Pins[6];

void resetControlArrays()
{
  for (int i=18; i>=0; i--) {
    control0Bits[i]=0;
    control1Bits[i]=0;
  }
  real0Pins[0]=0;
  real1Pins[0]=0;
  
}
void displayControlArrays()
{
  /*Lower and Upper left dots*/
  short int real_upper_left=getRealPin(upper_left_dot);
  short int real_lower_left=getRealPin(lower_left_dot);
  control1Bits[real_upper_left]=upper_left_enabled;
  control1Bits[real_lower_left]=lower_left_enabled;


  for (int i=18; i>=0; i--) { //17
    digitalWrite(regy.ds, control0Bits[i]);
    pulseClock(regy);
  }
  pulseLatch(regy);
  enableControl0(); 
  for (int i=1; i<=real0Pins[0]; i++) {
    digitalWrite(real0Pins[i], HIGH);
  }
  delayMicroseconds(DISPLAY_BRIGHTNESS);
  for (int i=1; i<=real0Pins[0]; i++) digitalWrite(real0Pins[i], LOW);
  
  resetRegisters(regy, 2);
  disableControl0(); 
  
  for (int i=18; i>=0; i--) { //17
    digitalWrite(regy.ds, control1Bits[i]);
    pulseClock(regy);
  }
  pulseLatch(regy);
  delayMicroseconds(10); enableControl1(); delayMicroseconds(10);
  for (int i=1; i<=real1Pins[0]; i++) {
    digitalWrite(real1Pins[i], HIGH);
  }
  delayMicroseconds(DISPLAY_BRIGHTNESS);
  for (int i=1; i<=real1Pins[0]; i++) digitalWrite(real1Pins[i], LOW);
  
  resetRegisters(regy, 2); 
  disableControl1(); 

}

int turnOnSegment(int poz, int segCode) {
  int hyphenIndex=1; //it's always the second character
  if (hyphenIndex!=-1) {
    int control=segmentCodes[poz][segCode][0], segmentArrayIndex=segmentCodes[poz][segCode][1];
    int pin=segments[segmentArrayIndex];
    if (control==0) {
      int realPin=getRealPin(pin);
      if (pin!=realPin) {
        control0Bits[realPin]=1;
      } else {
        real0Pins[++real0Pins[0]]=pin-27;
      }
    }
    if (control==1) {
      int realPin=getRealPin(pin);
      if (pin!=realPin) {
        control1Bits[realPin]=1;
      } else {
        real1Pins[++real1Pins[0]]=pin-27;
      }
    }
  }
}


int displayNumber (int poz, int nr) {
  ul segmentNeeded=digitCodes[nr];
  while (segmentNeeded!=0) {
    int cif=segmentNeeded%10; cif-=1;
    turnOnSegment(poz, cif);
    segmentNeeded/=10;
  }
  if (poz==3) {
    displayControlArrays();
    resetControlArrays();
  }

}

void showLowerLeftDot() {lower_left_enabled=true;}
void hideLowerLeftDot() {lower_left_enabled=false;}

void showUpperLeftDot() {upper_left_enabled=true;}
void hideUpperLeftDot() {upper_left_enabled=false;}

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