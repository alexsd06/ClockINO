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

int pinA = 2, pinB = 3, pinC = 4, pinD = 5, pinE = 6, pinF = 7, pinG = 8;
int D1 = 9, D2 = 10, D3 = 13, D4 = 12;
int segments[]={2, 3, 4, 5, 6, 7, 8};
int digits[]={9, 10, 13, 12};
int DISPLAY_BRIGHTNESS=5; //the smaller the number, the brighter the display

void setDisplayBrightness(int br)
{
  DISPLAY_BRIGHTNESS=br; //1...10
}

void initDisplayPins()
{
  for (int i=0; i<7; i++) pinMode(segments[i], OUTPUT);
  for (int i=0; i<4; i++) pinMode(digits[i], OUTPUT);
}

void setDigits(bool state)
{
  for (int i=0; i<4; i++) digitalWrite(digits[i], state);
}
void setPozState(int poz, bool state)
{
   digitalWrite(digits[poz], state);
}

bool digitMatrix[10][7]={
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

void setNumber(int cif)
{
  for (int i=0; i<7; i++) digitalWrite(segments[i], digitMatrix[cif][i]);
}

void displayNumber(int poz, int cif) {
  //poz: 1, 2, 3, 4
  //cif: 0-9
  setDigits(1);
  setPozState(poz, 0);
  setNumber(cif);
  delay(DISPLAY_BRIGHTNESS);

} 

void blink (int repeat, int del)
{
   displayNumber(0, 0);
   displayNumber(1, 0);
   displayNumber(2, 0);
   displayNumber(3, 0);
   for (int i=0; i<repeat; i++) {
     setDigits(1);
     delay(del);
     setDigits(0);
     delay(del);
   }
}
