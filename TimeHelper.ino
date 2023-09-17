
void initTime()
{
  Wire.begin();
  rtc.begin();
}

void displayTime()
{
  int secs=rtc.now().second();
  int mins=rtc.now().minute();
  int hours=rtc.now().hour();

  int h1=hours/10, h2=hours%10, m1=mins/10, m2=mins%10;
  int s1=secs/10, s2=secs%10;
  if (h1==1||h1==2) displayNumber(0, h1); 
  displayNumber(1, h2); displayNumber(2, m1); displayNumber(3, m2);
  if (disabledSmall==false) {
    displayNumber_small(0, s1); 
    displayNumber_small(1, s2); 
  }
}

void setTime(int cif) {
  if ((readTime==true||readAlarm==true)&&cif!=-1) {
    if (poz==0&&cif>=0&&cif<=2) {
      digit1=cif;
      poz++;
    }
    else if (poz==1) {
      if (digit1==2) {
        if(cif>=0&&cif<=3) {
          digit2=cif;
          poz++;
        }
      }
      else {
        digit2=cif;
        poz++;
      }
    }
    else if (poz==2) {
      if (cif>=0&&cif<=5) {
        digit3=cif;
        poz++;
      }
    }
    else if (poz==3) {
      digit4=cif;
      poz++;
    }
  }
} 

void displayDuringReadTime()
{
  if (digit1==1||digit1==2) displayNumber(0, digit1); 
  displayNumber(1, digit2); displayNumber(2, digit3); displayNumber(3, digit4);
}

void display()
{
  if (disabled==true) return;
  if (readTime==false&&readAlarm==false) displayTime();
  else displayDuringReadTime();
  if (readSecs==true) showLowerLeftDot();
  if (readBrightness==true) showUpperLeftDot();
  if (readAlarm==true) {
    showLowerLeftDot();
    showUpperLeftDot();
  }
  showColumn();
  disableDigits();
}

void decreaseSeconds() {
  int secs=rtc.now().second();
  int mins=rtc.now().minute();
  int hours=rtc.now().hour();

  secs--;

  if (secs<0) {
    secs=59;
    mins--;
    if (mins<0) {
      mins=59;
      hours--;
      if (hours<0) {
        hours=23;
      }
    }
  }

  rtc.begin();
  rtc.adjust(DateTime(YMD, hours, mins, secs));

}

void increaseSeconds() {
  int secs=rtc.now().second();
  int mins=rtc.now().minute();
  int hours=rtc.now().hour();
  
  secs++;

  if (secs>59) {
    secs=0;
    mins++;
    if (mins>59) {
      mins=0;
      hours++;
      if (hours>23) {
        hours=0;
      }
    }
  }
  //Year Month Day
  rtc.begin();
  rtc.adjust(DateTime(YMD, hours, mins, secs));

}

void disableTimeReading()
{
  if (poz==4) {
    ul hours=digit1*10+digit2;
    ul mins=digit3*10+digit4;
    if (readTime==true) {
      readTime=false;
      rtc.adjust(DateTime(YMD, hours, mins, 0));
    }
    if (readAlarm==true) {
      readAlarm=false;
      rtc.set_alarm(DateTime(YMD, hours, mins, 0), {1, 1, 0, 0});
      //rtc.on_alarm();
    }
    digit1=0; digit2=0; digit3=0; digit4=0;
    poz=0;
  }
}

void disableAlarm()
{
  //TODO: Clear alarm values.
  rtc.set_alarm(DateTime(YMD, -1, -1, -1), {1, 1, 0, 0});
  stopBuzzer();
}

void checkAlarm()
{
  int h=rtc.now().hour(), m=rtc.now().minute();
  int ah=rtc.get_alarm().hour(), am=rtc.get_alarm().minute();
  if (h>=ah&&m>=am) {
    ALARM_RUNNING=true;
  }
}
void alarm() {
  checkAlarm();
  Serial.println(rtc.get_alarm().hour());
  Serial.println(rtc.get_alarm().minute());
  if (ALARM_RUNNING==true) {;
    //Serial.println("Alarm running");
    blinkBuzzer();
  }
}