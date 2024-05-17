//#include <ShiftRegister74HC595.h>
//ShiftRegister74HC595<numberOfShiftRegisters> sr(serialDataPin, clockPin, latchPin);
struct ShiftRegister {int ds, shcp, stcp, srclr; /*SRCLR can be -1; */ };
struct ShiftRegister regy;
const int numberOfShiftRegisters = 2; // number of shift registers attached in series
const int serialDataPin = 10; // DS
const int clockPin = 8; // SHCP
const int latchPin = 9; // STCP
struct ShiftRegister initShiftRegister(int ds, int shcp, int stcp, int srclr)
{
  pinMode(ds, OUTPUT);
  pinMode(shcp, OUTPUT);
  pinMode(stcp, OUTPUT);
  pinMode(srclr, OUTPUT);
  digitalWrite(srclr, HIGH);
  ShiftRegister reg; reg.ds=ds; reg.shcp=shcp; reg.stcp=stcp; reg.srclr=srclr;
  return reg;
}

void pulse (int pin)
{
  digitalWrite(pin, HIGH);
  delayMicroseconds(1);
  digitalWrite(pin, LOW);
  delayMicroseconds(1);
}

void pulseClock(ShiftRegister reg) {pulse(reg.shcp);}
void pulseLatch(ShiftRegister reg) {pulse(reg.stcp);}

void resetRegisters (ShiftRegister reg, int count)
{
  digitalWrite(reg.ds, LOW);
  for (int i=0; i<8*count+2; i++) pulseClock(reg);
  pulseLatch(reg);
}

void initClockINORegisters()
{
  regy=initShiftRegister(serialDataPin, clockPin, latchPin, -1);
}
