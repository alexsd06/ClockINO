extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory() {
  int free_memory;
  if ((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  } else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
  }
  return free_memory;
}

ul last_memory_check=0;
void checkMemory()
{
  if (abs(millis()-last_memory_check)>5*1000) {
    Serial.print(F("[KMSG] Free memory: "));
    Serial.print(freeMemory()); Serial.println(F(" bytes!"));
    last_memory_check=millis();
  }
}