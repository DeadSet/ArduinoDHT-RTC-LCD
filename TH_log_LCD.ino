//Logs timestamp Temp and Humidity
//LCD display

#include "DHT.h" //for sensor
#include <Wire.h> //for RTC & RBGLCD Shield
#include "RTClib.h" //for RTC
#include <SD.h> // for SD card
#include <Adafruit_MCP23017.h> //for RGBLCD Shield
#include <Adafruit_RGBLCDShield.h> //for RGBLCD Shield

const int chipSelect = 10;

RTC_DS1307 rtc; //need to research this

#define DHTPIN 2 // input pin for sensor
#define DHTTYPE DHT22 //DHT library requires sensor type (AM2302)
//These are for LCB backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

//DHT pinnout (5V,DHTPIN,empty,Ground 10K pullup to 5V)

DHT dht(DHTPIN,DHTTYPE);
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

int last; // to track second value of last log/display


void setup(){
  Serial.begin(9600);
  Serial.println("dataFile Go");
 #ifdef AVR
   Wire.begin();
 #else
   Wire1.bring(); // this might be specific to Due
 #endif
   rtc.begin();
 
   if (! rtc.isrunning()){
     Serial.println("RTC is NOT running");
   }
  
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("Failed to Start SD Card");
  } else {
    Serial.println("SD Card Start");
  }
  
  dht.begin();
  lcd.begin(16,2); //send columns,rows

}

void loop(){
  DateTime now = rtc.now();
  
 
  
  if ((int) now.second() != last){  // Only operate once per second
    
    if ((int) now.second() % 6 == 0 ){  // Operate every 6 seconds
    
       
       last = (int)now.second();
       
//////////////////

        float h = dht.readHumidity();
        float t = dht.readTemperature(1); // 1 for degrees F 
               
        if (isnan(t) || isnan(h)) {
          Serial.println("DHT read fail");
        } else {
          
          Serial.print(now.year(), DEC);
          Serial.print('/');
          Serial.print(now.month(), DEC);
          Serial.print('/');
          Serial.print(now.day(), DEC);
          Serial.print(',');
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);
          Serial.print(':');
          Serial.print(now.second(), DEC);
          Serial.print(',');
          Serial.print(t);
          Serial.print(',');
          Serial.println(h);
         
          File dataFile = SD.open("THlog.txt", FILE_WRITE);

          if (dataFile) {  
    
            dataFile.print(now.year(), DEC);
            dataFile.print('/');
            dataFile.print(now.month(), DEC);
            dataFile.print('/');
            dataFile.print(now.day(), DEC);
            dataFile.print(',');
            dataFile.print(now.hour(), DEC);
            dataFile.print(':');
            dataFile.print(now.minute(), DEC);
            dataFile.print(':');
            dataFile.print(now.second(), DEC);
            dataFile.print(',');
            dataFile.print(t);
            dataFile.print(',');
            dataFile.println(h);
 
            dataFile.close();
            Serial.println("Write OK");
          } else {
              Serial.println("Error opening data file");
            
            
            }
        }   
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("DB");
          lcd.print(t);
          lcd.print("F");
          lcd.setCursor(9,0);
          lcd.print(now.month(), DEC);
          lcd.print("/");
          lcd.print(now.day(), DEC);
          lcd.setCursor(0,1);
          lcd.print("RH");
          lcd.print(h);
          lcd.print("%");
          lcd.setCursor(9,1);
          lcd.print(now.hour(), DEC);
          lcd.print(":");
          lcd.print(now.minute(), DEC);
          lcd.print(":");
          lcd.print(now.second(), DEC);
/////////////////
       
       
       
       
       
    }
   }
 }

