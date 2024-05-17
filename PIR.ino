#define movementDebug
void initPir()
{
  pinMode(PIR_PIN, INPUT);
}
unsigned long lastEnable=0;
const int timeOut=5*1000;
void checkPir()
{
  bool status=digitalRead(PIR_PIN);
  if (status&&!IR_REMOTE&&disabled) {
    lastEnable=millis();
    disabled=false;
    #ifdef movementDebug
    Serial.println(F("[OK] Movement detected!"));
    #endif
  }
  if (millis()>lastEnable+timeOut&&!IR_REMOTE&&!disabled) {
    disabled=true;
    #ifdef movementDebug
    Serial.println(F("[OK] Movement timed out! Disabling!"));
    #endif
  }
}