// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

#define DEBUG_RTC false

RTC_DS1307 rtc;

String get_DateTime_String (DateTime currentDateTime) {
    
    String sDateTime = "";
    
    sDateTime += currentDateTime.year();
    sDateTime += F(".");
    sDateTime += currentDateTime.month();
    sDateTime += F(".");
    sDateTime += currentDateTime.day();
    sDateTime += F(" - ");
    sDateTime += currentDateTime.hour();
    sDateTime += F(":");
    sDateTime += currentDateTime.minute();
    sDateTime += F(":");
    sDateTime += currentDateTime.second();
  
    return sDateTime;
}


int setup_Rtc() {

  int errorCode = 0;

  if(DEBUG_RTC){
    Serial.print("Rtc.h \nsetup_Rtc START \n");
  }
   
  if (! rtc.begin()) {
    if(DEBUG_RTC){
      Serial.print("ERROR: Couldn't find RTC \n");
    }
    errorCode += 1;
    while (1);
  }

  if (! rtc.isrunning()) {
    if(DEBUG_RTC){
      Serial.print("ERROR: RTC is NOT running! \n");
    }
    errorCode += 2;
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2017, 3, 17, 9, 36, 0));
  }

  
  if(DEBUG_RTC){
    Serial.print(get_DateTime_String(rtc.now()) + " \n");
    Serial.print("setup_Rtc END \n");
  }
  
  return errorCode;
}




