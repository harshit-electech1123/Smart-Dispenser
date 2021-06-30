#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define sensor A0
#define beep 6
int value;
int info=0;
int perc;
int callibration(){
      int sum=0;
  for(int i=0; i<10; i++){
  int v = analogRead(sensor);
  sum=sum+v;
  }
  return sum/10;
}
#include "TM1637.h"
#define IRsensor 12
#define DCwater_pump 13
#define CLK 8//Pins for TM1637       
#define DIO 9
TM1637 tm1637(CLK,DIO);
void pumpoff(){
    tm1637.display(1,0); 
    tm1637.display(2,0);
    digitalWrite(DCwater_pump,LOW);
    Serial.println("DC Pump is OFF Now!!");
    delay(500);
} 
void timer(){
    tm1637.display(1,0); 
    tm1637.display(2,3);
    delay(1000);
    tm1637.display(1,0); 
    tm1637.display(2,2);
    delay(1000);
    tm1637.display(1,0); 
    tm1637.display(2,1);
    delay(1000);
    tm1637.display(1,0); 
    tm1637.display(2,0);
}
void setup()
{
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);}
    display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Smart");
  display.display();
  display.setCursor(0,30);
    display.println("Sanitizer");
  display.display();
  delay(1500);
  pinMode(IRsensor, INPUT);
  pinMode(beep, OUTPUT);
  pinMode(DCwater_pump, OUTPUT);
  Serial.begin(9600);
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL); 
  //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
 
  delay(1500);//Delay to let system boot

}
int readPin = 0;
void loop()
{
 value=callibration();
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
 perc=map(value, 300, 570, 0, 100);
 Serial.println(value);

 
   if (perc<90&&perc>0){
    analogWrite(beep,170);
  }
  else if(perc>=90){
        digitalWrite(beep,LOW);
  }
  else{
    digitalWrite(beep,HIGH);
  }
 
  if(perc>=100){
    Serial.println("100%");
    display.println("full");
    display.display();
  }
  else if(perc<=0){
    Serial.println("0%");
    display.println("empty");
    display.display();
  }
  else{
    Serial.print("moisture percentage is ");
    Serial.print(perc);
    Serial.println("%");
//    display.print(perc);
//    display.display();
//    display.println("%");
//    display.display();
  if (perc<90){
    display.println("refill");
    display.display();
  }
  else if(perc>=90){
    display.println("full");
    display.display();
  }
  }
  delay(10);

 readPin = digitalRead(IRsensor);
 if (readPin == LOW)
{
 digitalWrite(DCwater_pump,HIGH);
 Serial.println("DC Pump is ON Now!!");
 delay(300);
 pumpoff();
 timer();
}
else
{
pumpoff();

}
}
