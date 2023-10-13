void initPir()
{
  pinMode(PIR_PIN, INPUT);
}
void checkPir()
{
  bool status=digitalRead(PIR_PIN);
  if (status&&!IR_REMOTE) {
    disabled=false;
  }
  if (!status&&!IR_REMOTE) {
    disabled=true;
  }
}