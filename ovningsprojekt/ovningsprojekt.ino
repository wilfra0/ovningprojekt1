/*
* Name: övningsprojekt
* Author: William Franzen
* Date: 2025-11-11
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display, 
* Further, it measures temprature with ds3231 and displays a mapped value to a 9g-servo-motor.
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include "U8glib.h"
#include <Servo.h>

// Init constants

// Init global variables

// Construct objects
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
Servo myservo; 


void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();
  myservo.attach(9);

  // Init Hardware
  rtc.begin();

  // Settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  oledWrite( getTime());
  servoWrite( getTemp() );
  showTempColor(getTemp()); 

  Serial.println(getTime());
  Serial.println(getTemp());


  delay(200);
}



//This function reads time from an ds3231 module and package the time as a String
//Parameters: Void
//Returns: time in hh:mm:ss as String
String getTime() {
  DateTime now = rtc.now();
  return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

/*
* This function takes temprature from ds3231 and returns as a float
*Parameters: Void
*Returns: temprature as float 
*/
float getTemp() {
  return rtc.getTemperature();
}

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text) {
    u8g.firstPage();
  do {
    u8g.setFont(u8g_font_helvB12);
    u8g.drawStr(0, 30, text.c_str());
  } while (u8g.nextPage());
}


/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value) {
  int servoAngle = map(value, 20, 30, 0, 180); 
  myservo.write(servoAngle);
}


// Defining the color variables and the pins on Arduino
int RED = 5; 
int GREEN = 6;
int BLUE = 7;

/*
* This function takes a temprature value and maps it to the corresponding value of red and blue on a LED
*Parameters - temp: temprature
*Returns void
*/
void showTempColor(float temp) {
  
  int value = map(temp, 20, 35, 0, 255);
  
  analogWrite(RED, value);       
  analogWrite(GREEN, 0); 
  analogWrite(BLUE, 255 - value);
}

