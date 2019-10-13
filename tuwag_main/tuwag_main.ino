/* Switches on two Relais (in paralell) once a weekday (in the morning)
 * Created by Rentouch GmbH 2019
 */

#include "RTClib.h"
RTC_DS3231 rtc;

// Define when the boot of the macmini should happen
const int startHour = 7; 
const int startMin = 01;
// Defines when we think is midnight (hour 1-2) used to reset flag
const int midnightHour = 1;
const int midnightMin = 01;


void setup() {
  delay(1000);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  while(1){
    if (!rtc.begin()) {
      Serial.println("Couldn't find RTC");
      delay(3000);
    }else{
      Serial.println("Fount RTC");
      delay(1000);  // Not sure why this is needed, but without the delay it sometimes hangs
      break;
    }
  }
}

// True if the relais was already switched on today
// gets set to False at midnight between 0-1
bool startedToday=false;  

void loop() {
  DateTime now = rtc.now();
  
  // Display current time via serial
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print("- startedToday:");
  Serial.print(startedToday);
  Serial.println();
  
  // Reset startedToday flag at midnight
  if (now.hour()==midnightHour & now.minute()>=midnightMin & now.minute()<midnightMin+2){
    startedToday=false;
  }

  // Start MacMini when its later than startHour/startMin and it was not already started
  if (now.hour()==startHour & now.minute()>=startMin & now.minute()<startMin+2 & !startedToday){
    startMac();
  }
  
  delay(2000);
}


void startMac(){
  // Switches the relais ON for 1 second to short the switch of the MacMini
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  startedToday=true;
}
