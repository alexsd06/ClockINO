void initBuzzer()
{
  pinMode(BUZZER_PIN, OUTPUT);
}

void playBuzzer() {
  tone(BUZZER_PIN, 1000);
}
void stopBuzzer() {
  noTone(BUZZER_PIN);
}
void blinkBuzzer()
{
  if (millis()%500<=250) {
      playBuzzer();
    }
    else {
      stopBuzzer();
    }
}