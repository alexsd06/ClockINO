int turnedOn[28]; //maxim 14 + turnedOn[0] care retine dimensiunea

int DISPLAY_BRIGHTNESS=1000;
//Digital Pin 3 needs repairs (breadbord pozition 6)
int controls[]={A4, A5};
////////////////0  1  2  3  4  5  6  7  8   9   10  11  12  13
int segments[]={2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1};

String segmentCodes[4][7]={ ///codes[digit][segment(as index of segments)]="activation code";
  // A      B      C      D      E      F      GA 
  {"1-1", "0-0", "0-3", "1-2", "0-2", "!-!", "0-1" }, //digit 0
  {"0-6", "0-4", "0-5", "1-5", "1-3", "1-6", "1-4" }, //digit 1
  {"1-7", "1-8", "1-9", "0-9", "0-10","0-7", "0-8" }, //digit 2
  {"0-13","0-11","0-12","1-12","1-10","1-13","1-11"}  //digit 4 
};
/*
int segmentCodesNew[4][7][2]={
  {{1, 1}, {0, 0}, {0, 3}, {1, 2}, {0, 2}, {-1, -1}, {0, 1}},
  {{0, 6}, {0, 4}, {0, 5}, {1, 5}, {1, 3}, {1,   6}, {1, 4}},
  {{1, 7}, {1, 8}, {1, 9}, {0, 9}, {0,10}, {0,   7}, {0, 8}},
  {{0,13}, {0,11}, {0,12}, {1,12}, {1,10}, {1,  13}, {1,11}}
};
*/
String digitCodes[10]={
  //0        1       2        3       4        5        6        7        8         9
  "ABCDEF", "BC", "ABGED", "ABGCD", "FGBC", "AFGCD", "AFEDGC", "ABC", "ABCDEFG", "GFABCD"
};
int column=A2;

void initDisplay()
{
  for (int i=0; i<sizeof(controls)/sizeof(int); i++) pinMode(controls[i], OUTPUT);
  for (int i=0; i<sizeof(segments)/sizeof(int); i++) pinMode(segments[i], OUTPUT);
  for (int i=0; i<sizeof(turnedOn)/sizeof(int); i++) turnedOn[i]=0;
  pinMode(column, OUTPUT);
}

void setDisplayBrightness(int bright) {
  DISPLAY_BRIGHTNESS=bright;
}

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

void turnOnSegmentPin(int pin) {
  digitalWrite(pin, HIGH);
  turnedOn[++turnedOn[0]]=pin;
}
void turnOffSegmentPin(int pin) {
  digitalWrite(pin, LOW);
}

void disableDigits()
{
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
  digitalWrite(column, HIGH);
  turnedOn[++turnedOn[0]]=column;
  delayMicroseconds(DISPLAY_BRIGHTNESS);
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
    if (control==0) enableControl0();
    if (control==1) enableControl1();
    turnOnSegmentPin(segments[segmentArrayIndex]);
    delayMicroseconds(DISPLAY_BRIGHTNESS);
  }
}

/*
int turnOnSegmentNew(int poz, char seg) {
  int segCode=(int)(seg-'A');
  int control=segmentCodesNew[poz][segCode][0];
  int segmentArrayIndex=segmentCodesNew[poz][segCode][1];
  disableDigits();
  if (control==0) enableControl0();
  if (control==1) enableControl1();
  turnOnSegmentPin(segments[segmentArrayIndex]);
  delayMicroseconds(DISPLAY_BRIGHTNESS);

}
*/

int displayNumber (int poz, int nr) {
  String segmentsNeeded=digitCodes[nr];
  for (int i=0; i<segmentsNeeded.length(); i++) {
    char segment=segmentsNeeded[i];
    turnOnSegment(poz, segment);
  }
  showColumn();
}
