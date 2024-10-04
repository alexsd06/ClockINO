int months_days[]={0, 31, -1, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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
  if (readTime==false&&readAlarm==false) {
    displayTime();
  }
  else {
    displayDuringReadTime();
  }
  if (readSecs==true) {
    showLowerLeftDot();
  }
  if (readHours==true) {
    showUpperLeftDot();
  }
  if (readMinutes==true) {
    showLowerLeftDot();
    showUpperLeftDot();
  }
  if (readAlarm==true) {
    showLowerLeftDot();
    showUpperLeftDot();
  }
  //////////////////////////
  if (!readMinutes&&!readAlarm&&!readSecs) {
    hideLowerLeftDot();
  }
  if (!readMinutes&&!readAlarm&&!readHours) {
    hideUpperLeftDot();
  }
  /////////////////////////
  showColumn();
  disableDigits();
}

void decreaseHours()
{
  if (!readHours) return;
  int secs=rtc.now().second();
  int mins=rtc.now().minute();
  int hours=rtc.now().hour();
  hours--;
  if (hours<0) hours=23;
  rtc.begin();
  rtc.adjust(DateTime(YMD, hours, mins, secs));
}

void increaseHours()
{
  if (!readHours) return;
  int secs=rtc.now().second();
  int mins=rtc.now().minute();
  int hours=rtc.now().hour();
  hours++;
  if (hours>23) hours=0;
  rtc.begin();
  rtc.adjust(DateTime(YMD, hours, mins, secs));
}

void decreaseSeconds() {
  if (!readSecs) return;
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
  if (!readSecs) return;
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

void decreaseMinutes()
{
  if (!readMinutes) return;
  int secs=rtc.now().second();
  int mins=rtc.now().minute();
  int hours=rtc.now().hour();

  mins--;
  if (mins<0) {
    mins=59;
    hours--;
    if (hours<0) {
      hours=23;
    }
  }

  rtc.begin();
  rtc.adjust(DateTime(YMD, hours, mins, secs));
}

void increaseMinutes()
{
  if (!readMinutes) return;
  int secs=rtc.now().second();
  int mins=rtc.now().minute();
  int hours=rtc.now().hour();

  mins++;
  if (mins>59) {
    mins=0;
    hours++;
    if (hours>23) {
      hours=0;
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
      int year=rtc.now().year();
      int month=rtc.now().month();
      int day=rtc.now().day();
      if (hours<rtc.now().hour()&&mins<rtc.now().minute()) {
        if (year%4==0&&year%100!=0) months_days[2]=29; //bisect
        else if (year%400==0) months_days[2]=29; //bisect
        else months_days[2]=28; //normal
        day++;
        if (day>months_days[month]) {
          day=1;
          month++;
        }
        if (month>12) {
          month=1;
          year++;
        }
        Serial.print(F("[OK] Setted alarm for next day!"));
      }
      else {
        Serial.println(F("[OK] Setted alarm for today!"));
      }
      rtc.set_alarm(DateTime(year, month, day, hours, mins, 0), {1, 1, 1, 1});
      Serial<<F("Now: ")<<rtc.now().year()<<F(" ")<<rtc.now().month()<<F(" ")<<rtc.now().day()<<F(" ")<<rtc.now().hour()<<F(" ")<<rtc.now().minute()<<F("\n");
      Serial<<F("Alarm: ")<<rtc.get_alarm().year()<<F(" ")<<rtc.get_alarm().month()<<F(" ")<<rtc.get_alarm().day()<<F(" ")<<rtc.get_alarm().hour()<<F(" ")<<rtc.get_alarm().minute()<<F("\n");
     //TODO: Make it work at the end of the month and between years.
    }
    digit1=0; digit2=0; digit3=0; digit4=0;
    poz=0;
  }
}

void disableAlarm()
{
  //TODO: Clear alarm values.
  rtc.set_alarm(DateTime(-1, -1, -1, -1, -1, -1), {1, 1, 0, 0});
  ALARM_RUNNING=false;
  stopBuzzer();
}

void checkAlarm()
{
  int h=rtc.now().hour(), m=rtc.now().minute(), d=rtc.now().day(), mo=rtc.now().month(), y=rtc.now().year();
  int ah=rtc.get_alarm().hour(), am=rtc.get_alarm().minute(), ad=rtc.get_alarm().day(), amo=rtc.get_alarm().month(), ay=rtc.get_alarm().year();
  if (h>=ah&&m>=am&&d>=ad) {//&&mo>=amo&&y>=ay) {
    ALARM_RUNNING=true;
  }
}
void alarm() {
  checkAlarm();
  if (ALARM_RUNNING==true) blinkBuzzer();
}