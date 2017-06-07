
//Code to run 6 digit Nixie clock ArduiNIX V3
//5 buttons hooked up to A0 with different resistance values
//DS3231 connected to SDA and SCL
//Clock defaults to 12:00:00 on every startup because I opted to
    //do away with battery backup on DS3231. This can be altered in setup
//Aditya Jituri, June 2017
//Some functions and documentation thereof from code from ArduiNIX by Jeremy Howa
#include "Time.h"
#include "DS3231.h"
#include "Wire.h"
DS3231 rtc(SDA,SCL);
Time t;

#define DEBUG_ON   false

// SN74141 (1)
int ledPin_0_a = 2;                
int ledPin_0_b = 3;
int ledPin_0_c = 4;
int ledPin_0_d = 5;
// SN74141 (2)
int ledPin_1_a = 6;                
int ledPin_1_b = 7;
int ledPin_1_c = 8;
int ledPin_1_d = 9;

// anod pins
int ledPin_a_1 = 10;
int ledPin_a_2 = 11;
int ledPin_a_3 = 12;

//State variables for button
int b;
int lb = 0;

//variables for time DIGITS
int hr1 = 1;
int hr2 = 2;
int m1=0;
int m2=0;
int s1=0;
int s2=0;

//variables for time
int twelvehr;
int minutes;
int seconds;

void setup() 
{
  pinMode(ledPin_0_a, OUTPUT);      
  pinMode(ledPin_0_b, OUTPUT);      
  pinMode(ledPin_0_c, OUTPUT);      
  pinMode(ledPin_0_d, OUTPUT);    
  
  pinMode(ledPin_1_a, OUTPUT);      
  pinMode(ledPin_1_b, OUTPUT);      
  pinMode(ledPin_1_c, OUTPUT);      
  pinMode(ledPin_1_d, OUTPUT);      
  
  pinMode(ledPin_a_1, OUTPUT);      
  pinMode(ledPin_a_2, OUTPUT);      
  pinMode(ledPin_a_3, OUTPUT);     

  pinMode(A0, INPUT);


  rtc.begin();
  rtc.setTime(12,00,00);
  if( DEBUG_ON )
  {
    Serial.begin(9600);
  }
}

////////////////////////////////////////////////////////////////////////
//
// DisplayNumberSet
//
////////////////////////////////////////////////////////////////////////
void DisplayNumberSet( int anod, int num1, int num2 )
{
  int anodPin;
  int a,b,c,d;
  
  // set defaults.
  a=0;b=0;c=0;d=0; // will display a zero.
  anodPin =  ledPin_a_1;     // default on first anod.
  
  // Select what anod to fire.
  switch( anod )
  {
    case 0:    anodPin =  ledPin_a_1;    break;
    case 1:    anodPin =  ledPin_a_2;    break;
    case 2:    anodPin =  ledPin_a_3;    break;
  }  
  
  // Load the a,b,c,d.. to send to the SN74141 IC (1)
  switch( num1 )
  {
    case 0: a=0;b=0;c=0;d=0;break;
    case 1: a=1;b=0;c=0;d=0;break;
    case 2: a=0;b=1;c=0;d=0;break;
    case 3: a=1;b=1;c=0;d=0;break;
    case 4: a=0;b=0;c=1;d=0;break;
    case 5: a=1;b=0;c=1;d=0;break;
    case 6: a=0;b=1;c=1;d=0;break;
    case 7: a=1;b=1;c=1;d=0;break;
    case 8: a=0;b=0;c=0;d=1;break;
    case 9: a=1;b=0;c=0;d=1;break;
  }  
  
  // Write to output pins.
  digitalWrite(ledPin_0_d, d);
  digitalWrite(ledPin_0_c, c);
  digitalWrite(ledPin_0_b, b);
  digitalWrite(ledPin_0_a, a);

  // Load the a,b,c,d.. to send to the SN74141 IC (2)
  switch( num2 )
  {
    case 0: a=0;b=0;c=0;d=0;break;
    case 1: a=1;b=0;c=0;d=0;break;
    case 2: a=0;b=1;c=0;d=0;break;
    case 3: a=1;b=1;c=0;d=0;break;
    case 4: a=0;b=0;c=1;d=0;break;
    case 5: a=1;b=0;c=1;d=0;break;
    case 6: a=0;b=1;c=1;d=0;break;
    case 7: a=1;b=1;c=1;d=0;break;
    case 8: a=0;b=0;c=0;d=1;break;
    case 9: a=1;b=0;c=0;d=1;break;
  }
  
  // Write to output pins
  digitalWrite(ledPin_1_d, d);
  digitalWrite(ledPin_1_c, c);
  digitalWrite(ledPin_1_b, b);
  digitalWrite(ledPin_1_a, a);

  // Turn on this anod.
  digitalWrite(anodPin, HIGH);   

  // Delay
  // NOTE: With the difference in Nixie bulbs you may have to change
  //       this delay to set the update speed of the bulbs. If you 
  //       dont wait long enough the bulb will be dim or not light at all
  //       you want to set this delay just right so that you have 
  //       nice bright output yet quick enough so that you can multiplex with
  //       more bulbs.
  delay(1);
  
  // Shut off this anod.
  digitalWrite(anodPin, LOW);
}

////////////////////////////////////////////////////////////////////////
//
// DisplayNumberString
// Use: passing an array that is 8 elements long will display numbers
//      on a 6 nixie bulb setup.
//
////////////////////////////////////////////////////////////////////////
void DisplayNumberString( int zero, int one, int two, int three, int four, int five )
{
  // bank 1 (bulb 0,3)5,2
  DisplayNumberSet(0,five,two);   
  // bank 2 (bulb 1,4)4,2
  DisplayNumberSet(1, four ,one);   
  // bank 3 (bulb 2,5)3,0
  DisplayNumberSet(2,three,zero);   
}



////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
void loop()     
{  
  t = rtc.getTime();
  twelvehr = t.hour;
  //Remove from here to...
  if(twelvehr>11){
    if (twelvehr !=12){
      twelvehr = twelvehr - 12;
    }
  }
  // here for 24 hour time and make adjustments noted below
  int minutes = t.min;
  int seconds = t.sec;
  hr1 = twelvehr/10;
  hr2 = twelvehr%10;
  m1 = minutes/10;
  m2 = minutes%10;
  s1 = seconds/10;
  s2 = seconds%10;

  int n = analogRead(A0);
  if (n > 1015 && n < 1025){
    b = 1;
  } else if (n > 1000 && n < 1015){
    b = 2;
  } else if (n > 925 && n <935){
    b = 3;
  } else if (n > 765 && n < 775){
    b = 4;
  } else if (n > 625 && n < 635){
    b = 5;
  } else if (n < 500){
    b = 0;
  }
  if (b != lb){
    if(b == 1){
      rtc.setTime(twelvehr, minutes, 0);
    }
    if(b == 2){
      //if using 24hr time, add case for twelvhr = 0, like with minutes = 0
      rtc.setTime(twelvehr - 1, minutes, seconds);
    }
    if(b == 3){
      //if using 24hr time, add case for twelvhr = 23, like with minutes = 59
      rtc.setTime(twelvehr + 1, minutes, seconds);
    }
    if(b == 4){
      if (minutes == 0){
        rtc.setTime(twelvehr, 59, seconds);
      } else{
        rtc.setTime(twelvehr, minutes - 1, seconds);
      }
    }
    if(b == 5){
      if (minutes == 59){
        rtc.setTime(twelvehr, 0, seconds);
      } else{
        rtc.setTime(twelvehr, minutes + 1, seconds);
      }
    }
  }
  
  if(hr2 == 8 && m1 == 0 && m2 == 0){
    DisplayNumberString(s2, s2, s2, s2, s2, s2);
  } else {
    DisplayNumberString( hr1,hr2,m1,m2,s1,s2 );
  }
  lb = b;
}
