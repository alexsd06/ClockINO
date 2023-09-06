#include <IRremote.hpp>
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

int pinA = 2;
int pinB = 3;
int pinC  = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
int  D1 = 9;
int D2 = 10;
int D3 = 13; //fostul 11
int D4 = 12;
void displayNumber(int poz, int cif) {
  //poz: 1, 2, 3, 4
  //cif: 0-9
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  if (poz==1) digitalWrite(D1, LOW);
  if (poz==2) digitalWrite(D2, LOW);
  if (poz==3) digitalWrite(D3, LOW);
  if (poz==4) digitalWrite(D4, LOW);

  if (cif==0) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH); 
    digitalWrite(pinC, HIGH); 
    digitalWrite(pinD, HIGH);  
    digitalWrite(pinE, HIGH);  
    digitalWrite(pinF, HIGH); 
    digitalWrite(pinG, LOW);
  }
  if (cif==1) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH); 
    digitalWrite(pinC, HIGH); 
    digitalWrite(pinD, LOW);  
    digitalWrite(pinE, LOW);  
    digitalWrite(pinF, LOW); 
    digitalWrite(pinG, LOW);
  }
  if (cif==2) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH); 
    digitalWrite(pinC, LOW); 
    digitalWrite(pinD, HIGH);  
    digitalWrite(pinE, HIGH);  
    digitalWrite(pinF, LOW); 
    digitalWrite(pinG, HIGH);
  }
  if (cif==3) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH); 
    digitalWrite(pinC, HIGH); 
    digitalWrite(pinD, HIGH);  
    digitalWrite(pinE, LOW);  
    digitalWrite(pinF, LOW); 
    digitalWrite(pinG, HIGH);
  }
  if (cif==4) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH); 
    digitalWrite(pinC, HIGH); 
    digitalWrite(pinD, LOW);  
    digitalWrite(pinE, LOW);  
    digitalWrite(pinF, HIGH); 
    digitalWrite(pinG, HIGH);
  }
  if (cif==5) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW); 
    digitalWrite(pinC, HIGH); 
    digitalWrite(pinD, HIGH);  
    digitalWrite(pinE, LOW);  
    digitalWrite(pinF, HIGH); 
    digitalWrite(pinG, HIGH);
  }
  if (cif==6) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW); 
    digitalWrite(pinC, HIGH); 
    digitalWrite(pinD, HIGH);  
    digitalWrite(pinE, HIGH);  
    digitalWrite(pinF, HIGH); 
    digitalWrite(pinG, HIGH);
  }
  if (cif==7) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH); 
    digitalWrite(pinC, HIGH); 
    digitalWrite(pinD, LOW);  
    digitalWrite(pinE, LOW);  
    digitalWrite(pinF, LOW); 
    digitalWrite(pinG, LOW);
  }
  if (cif==8) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH); 
    digitalWrite(pinC, HIGH); 
    digitalWrite(pinD, HIGH);  
    digitalWrite(pinE, HIGH);  
    digitalWrite(pinF, HIGH); 
    digitalWrite(pinG, HIGH);
  }
  if (cif==9) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH); 
    digitalWrite(pinC, HIGH); 
    digitalWrite(pinD, HIGH);  
    digitalWrite(pinE, LOW);  
    digitalWrite(pinF, HIGH); 
    digitalWrite(pinG, HIGH);
  }
}

int RECV_PIN = 11;
int count=0;

// the setup routine  runs once when you press reset:
void setup() {                
  // initialize  the digital pins as outputs.
  pinMode(pinA, OUTPUT);     
  pinMode(pinB,  OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG,  OUTPUT);   
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3,  OUTPUT);  
  pinMode(D4, OUTPUT);  

  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN);

}

void increment()
{
  if (count==10) count=0;
  displayNumber(4, count);
  count+=1;
  delay(1000);
}

int irToNumber(unsigned long data)
{
  String codeArray[10]={"ef10fb04", "ee11fb04", "ed12fb04", "ec13fb04", "eb14fb04", "ea15fb04", "e916fb04", "e817fb04", "e718fb04", "e619fb04"};
  String hexCode=String(data, HEX);
  Serial.println(hexCode);
  for (int i=0; i<10; i++) {
    if (hexCode==codeArray[i])
      return i;
  }
  return -1;
}

// the loop routine runs over  and over again forever:
void loop() {
  //increment(); 
  if (IrReceiver.decode()) {
      unsigned long rawData=IrReceiver.decodedIRData.decodedRawData;
      IrReceiver.resume(); // Enable receiving of the next value
      if (rawData==0) return;
      int cif=irToNumber(rawData);
      if (cif!=-1) {
        displayNumber(4, cif);
      }
      else Serial.println("Invalid key press");
  }
  
}