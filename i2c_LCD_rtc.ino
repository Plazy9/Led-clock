#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int ledPin = 13;                 // LED connected to digital pin 13

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

// Define Connections to 74HC595

// ST_CP pin 12
const int latchPin = 8;
// SH_CP pin 11
const int clockPin = 12;
// DS pin 14
const int dataPin = 11;

void ledSwitch(int numberToDisplay){
  byte myByte = B0;
  for (int i = 0; i<numberToDisplay; i++){
    myByte = myByte << 1; 
    myByte = myByte | B00000001;
  }
  //Serial.print(myByte, DEC);
  digitalWrite(latchPin, LOW);
  //delay(1000);
  // Shift out the bits
  shiftOut(dataPin, clockPin, MSBFIRST, myByte);
  // ST_CP HIGH change LEDs
  digitalWrite(latchPin, HIGH);
}

void setup()
{
  Serial.begin(115200);
  //pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  //Shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  

  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

}


void loop()
{
  //digitalWrite(ledPin, HIGH);   // sets the LED on
  //delay(2000);                  // waits for a second
  //digitalWrite(ledPin, LOW);    // sets the LED off
  //delay(200);                  // waits for a second

  DateTime now = RTC.now();

  //lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Date: "+String(now.year())+"."+ String(now.month()) +"." + String(now.day()));
  
  lcd.setCursor(0,2);
  String hour = "";
  if (now.hour() < 10){
    hour = "0" + String(now.hour());
  }else{
    hour = String(now.hour());
  }
  String min = "";
  if (now.minute() < 10){
    min = "0" + String(now.minute());
  }else{
    min = String(now.minute());
  }
  String sec = "";
  if (now.second() < 10){
    sec = "0" + String(now.second());
  }else{
    sec = String(now.second());
  }
  lcd.print("Time: "+hour+":"+ min +":" + sec );

  ledSwitch(atoi(sec.c_str())%10);
  
/*
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println(); 
*/
  delay(1000);

}
