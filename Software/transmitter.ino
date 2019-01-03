#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <IRLibSendBase.h>    
#include <IRLib_P01_NEC.h>    
#include <IRLib_P02_Sony.h>   


const int buttonPin = 12;
int buttonState = 0; 

uint16_t press_time = 0;
const uint16_t o1Time = 100;
const uint16_t o2Time = 500;

uint8_t currentScreen = 1;

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 6, /* data=*/ 5, /* cs=*/ 2, /* dc=*/ 7, /* reset=*/ 4);

IRsendSony mySonySender;
IRsendNEC myNECSender;

typedef struct
{
   uint8_t numBytes;
   uint32_t hexCode;
   String protocol;
} screen;

  screen screenArr[5];
 
void setup(void) 
{
  u8g2.begin();
  pinMode(buttonPin,INPUT_PULLUP);
  Serial.begin(9600);

  //homescreen - 0

  //TV - 1
  screenArr[1].numBytes = 32;
  screenArr[1].hexCode = 0x40BE629D;
  screenArr[1].protocol = "N";
  
  //lights - 2 
  screenArr[2].numBytes = 20;
  screenArr[2].hexCode = 0xa8bca;
  screenArr[2].protocol = "S";
   
  //timer - 3
 
    u8g2.clearBuffer();
    displayStart();
    u8g2.sendBuffer();

  displayCurrentScreen(currentScreen);
}

void loop(void) 
{
  //display screen once
  
  //wait for button press then act if so
  while (digitalRead(buttonPin) == LOW)
  { 
    delay(50);
    press_time = press_time + 50;  
  }
  if (press_time >= o2Time) //longer than 500 ms
  {
     if(currentScreen == 3)
     {
        currentScreen = 1;
     }
     else
     {
        currentScreen++;
     }
     displayCurrentScreen(currentScreen);
     press_time = 0;
  }
  else if(press_time >= o1Time) //longer than 100ms
  {
     sendSignal(currentScreen);
  }
   press_time = 0;
  
}

void displayStart()
{
  Serial.print("\nOn display start");
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_t0_17b_tr);
  u8g2.drawStr(30,20, "LGxMoney");
  u8g2.drawLine(25, 25, 105, 25);
  u8g2.setFont(u8g2_font_shylock_nbp_tr); // choose a suitable font
  u8g2.drawStr(38,41,"starting...");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1300);
}

void displayCurrentScreen(int index)
{ 
  u8g2.clearBuffer();
  
  u8g2.setFont(u8g2_font_t0_16b_te);

  if(index == 1)
  {
    u8g2.drawStr(35,15, "TV Power");
  }
  if(index == 2)
  {
     u8g2.drawStr(35,15, "Light Switch");
  }
  if(index == 3)
  {
     u8g2.drawStr(35,15, "Timer");
  }
  u8g2.sendBuffer();
}

void sendSignal(int index)
{
  String protocol = screenArr[index].protocol;
  uint32_t hexMessage = screenArr[index].hexCode;
  uint8_t bytes = screenArr[index].numBytes;
  
  if( protocol == "N" )
  {
    Serial.println(F("Sent Signal."));
    Serial.print("Sending an N!");
    myNECSender.send(hexMessage, bytes);
  }
  else if( protocol == "S" )
  {
    Serial.println(F("Sent Signal."));
    Serial.print("Sending an S!");
    mySonySender.send(hexMessage, bytes);
  }
  
  
  u8g2.setFont(u8g2_font_shylock_nbp_tr);
  u8g2.drawStr(10,40, "- sent Light Signal");
  u8g2.sendBuffer();
  delay(200);
  u8g2.clearBuffer();
  displayCurrentScreen(index);
  u8g2.sendBuffer();
}

void displayTimer()
{}
 /* Serial.print(F("On display Timer"));
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_t0_16b_te);
    u8g2.drawStr(35,15, "TIMER: ");
    u8g2.sendBuffer();
  while (currentScreen == 3)
  {
     while (digitalRead(buttonPin) == LOW)
     { 
      delay(50);
      press_time = press_time + 50;  
     }
     if (press_time >= o3Time) //longer than 1 second - power-saver/reset
     {
        Serial.print(F("PowerSaver"));
        powerSaverMode();
     }
     else if (press_time >= o2Time) //longer than 500 ms
     {
       currentScreen = 4;
     }
     else if(press_time >= o1Time) //longer than 100ms
     {
        u8g2.setFont(u8g2_font_shylock_nbp_tr);
        timer();
        press_time = 0; 
        displayTimer();
     }
    press_time = 0;
  }
}

float timer()
{
  long ms = 0;
  int seconds = 0;
  int minutes = 0;
  while(digitalRead(buttonPin) == HIGH)
  {
    delay(98.95);
    ms += 100;
    if((ms % 1000) == 0)
    { seconds++;
      if((seconds % 60) == 0)
      {
        minutes++;
        seconds = 0;
      }
      update(seconds, minutes);
    }  
  } 
  delay(1000);
}

void update(int seconds, int minutes)
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_10x20_tn);
  u8g2.setCursor(40, 40);
  u8g2.print(minutes);
  u8g2.print(":");
  if(seconds < 10)
  {
    u8g2.print("0");
  }
  u8g2.print(seconds);
  u8g2.sendBuffer();
}
*/
