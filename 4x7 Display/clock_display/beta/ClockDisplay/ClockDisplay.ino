#define ul unsigned long

int cnt=0;
bool state=0;

ul prevMilli, prevSecs, prevMins;

void setup() {
  
  
  Serial.begin(9600);

  initDisplay();

  prevMilli=millis();
  prevSecs=prevMilli/1000;
  prevMins=prevSecs/60;
}


void loop() {
  if (cnt==10) cnt=0;
  displayNumber(0, ((int)state)+1);
  displayNumber(1, cnt);
  displayNumber(2, cnt);
  displayNumber(3, cnt);
  ul milliseconds=millis();
  ul seconds=milliseconds/1000;
  ul minutes=seconds/60;
  if (seconds!=prevSecs) {
    cnt++;
    state=!state;
  }

  prevMilli=milliseconds;
  prevSecs=seconds;
  prevMins=minutes;
}
