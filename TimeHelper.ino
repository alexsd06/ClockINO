
void initTime()
{
  //rtc.enableAlarm();
  //attachInterrupt(0, alarmIntrerrupt, FALLING);
}

void displayTime()
{
  ul secs=rtc.getSecond();
  ul mins=rtc.getMinute();
  ul hours=rtc.getHour();
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
  int secs=rtc.getSecond();
  int mins=rtc.getMinute();
  int hours=rtc.getHour();
  
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

  rtc.initClock();
  rtc.setTime(hours, mins, secs);

}

void increaseSeconds() {
  int secs=rtc.getSecond();
  int mins=rtc.getMinute();
  int hours=rtc.getHour();
  
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

  rtc.initClock();
  rtc.setTime(hours, mins, secs);

}

void disableTimeReading()
{
  if (poz==4) {
    ul hours=digit1*10+digit2;
    ul mins=digit3*10+digit4;
    if (readTime==true) {
      readTime=false;
      rtc.initClock();
      rtc.setTime(hours, mins, 0);
    }
    if (readAlarm==true) {
      readAlarm=false;
      rtc.setAlarm(mins, hours, 100, 100);
    }
    digit1=0; digit2=0; digit3=0; digit4=0;
    poz=0;
  }
}

void alarmIntrerrupt()
{
  ALARM_RUNNING=true;
  //Serial.println("Alarm on");
  rtc.clearAlarm();
}

void alarm() {
  if (rtc.getSecond()%2==0) {
    playBuzzer();
  }
  else {
    stopBuzzer();
  }
}