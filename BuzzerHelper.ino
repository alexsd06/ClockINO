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