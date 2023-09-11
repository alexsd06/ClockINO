void initTime()
{
  prevMili=millis();
  prevSec=prevMili/1000;
  prevMin=prevSec/60;
}

void displayTime()
{
  ul secs=timeInSecs%60;
  ul mins=(timeInSecs/60)%60;
  ul timeInMins=timeInSecs/60;
  ul hours=timeInMins/60;
  if (hours==24) {
    timeInSecs=0;
    displayTime();
  }
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
  if (readTime==true&&cif!=-1) {
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
  if (readTime==false) {
    if (disabled==false) displayTime();
  }
  else {
    if (disabled==false) displayDuringReadTime();
  }
  if (readSecs==true) showLowerLeftDot();
  if (readBrightness==true) showUpperLeftDot();
  showColumn();
}

void disableTimeReading()
{
  if (poz==4) {
    readTime=false;
    ul hours=digit1*10+digit2;
    ul mins=digit3*10+digit4;
    timeInSecs=(hours*3600)+(mins*60);
    digit1=0; digit2=0; digit3=0; digit4=0;
    poz=0;
  }
}

void increaseTime()
{
  ul micro=micros();
  ul mili=micro/1000;
  ul seconds=mili/1000;
  ul mins=seconds/60;
  if (readTime==false) {
    if (seconds!=prevSec) {
      timeInSecs++;
    }
  }
  prevMili=mili;
  prevSec=seconds;
  prevMin=mins;
}
