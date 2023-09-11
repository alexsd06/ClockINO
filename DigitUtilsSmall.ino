/*
  https://ardushop.ro/en/
  https://www.hackster.io/fradut/timer-with-cd74hc164e-and-led-display-4-digits-7-segments-1e609d
  Showing number 0-9 on a Common Anode 7-segment LED display
  Displays the numbers 0-9 on the display, with one second inbetween.
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---
    D
  This example code is in the public domain.
*/
// https://ardushop.ro/en/electronics/191-4x7-segment-led-display.html?search_query=afisaj&results=2
// https://www.hackster.io/aboda243/how-to-use-4-digit-7-segment-without-library-b8e014
// Pin 2-8 is connected  to the 7 segments of the display.
// http://nicuflorica.blogspot.com/2013/07/afisaje-led-cu-7-segmente-si-arduino-ii.html

int segments_small[]={43, 44, 45, 46, 47, 48, 49};
int digits_small[]={50, 51, 52, 53};
int DP_small=42;
int DISPLAY_BRIGHTNESS_small=500; //the smaller the number, the brighter the display

void setDisplayBrightness_small(int br) {DISPLAY_BRIGHTNESS_small=br;}

int getDisplayBrightness_small() {return DISPLAY_BRIGHTNESS_small;}


void initDisplayPins_small()
{
  for (int i=0; i<7; i++) pinMode(segments_small[i], OUTPUT);
  for (int i=0; i<4; i++) pinMode(digits_small[i], OUTPUT);
  pinMode(DP_small, OUTPUT);
}
void setDigits_small(bool state) {for (int i=0; i<4; i++) digitalWrite(digits_small[i], !state);} //Turn on/off all control pins 
void disableDigits_small() //Turn off all control pins and segments
{
  for (int i=0; i<4; i++) digitalWrite(digits_small[i], HIGH);
  for (int i=0; i<7; i++) digitalWrite(segments_small[i], LOW);;
}
void setDigitToWrite_small(int poz, bool state) {digitalWrite(digits_small[poz], !state);}

bool digitMatrix_small[10][7]={
  {1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1},
  {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1},
  {1, 0, 1, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 0, 1, 1}
};

void setNumber_small(int cif)
{
  for (int i=0; i<7; i++) {
    digitalWrite(segments_small[i], digitMatrix_small[cif][i]);
  }
}
void displayDecimal_small(int poz)
{
  setDigits_small(0);
  digitalWrite(DP_small, HIGH);
  digitalWrite(digits_small[poz], LOW);
  delayMicroseconds(DISPLAY_BRIGHTNESS_small);
}
void displayNumber_small(int poz, int cif) {
  //poz: 1, 2, 3, 4
  //cif: 0-9
  disableDigits_small();
  setDigitToWrite_small(poz, 1);
  setNumber_small(cif);
  delayMicroseconds(DISPLAY_BRIGHTNESS_small);
} 

void blink_small (int repeat, int del)
{
   bool disabled_blink_small=false;
   while (repeat!=0) {
    unsigned long m=millis();
    if (m%(del*2)>=0&&m%(del*2)<=del) {
      displayNumber(1, 0);
      displayNumber(2, 0);
      displayNumber(3, 0);
      showColumn();
      disabled_blink_small=false;
    }
    else {
      if (disabled==false) {
        disableDigits();
        disabled_blink_small=true;
        repeat--;
      }
    }
   }
}
