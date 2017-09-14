#include "Rtc.h"
#include <NanoESP.h>
#include <NanoESP_HTTP.h> //special library for http-connection
#include <SoftwareSerial.h>
#include "Websites.h" //websites in a special file as progmem 
#include "SunRiseSunSet.h"
#include "Battery.h"
#include "Door.h"
#include "Temperature.h"

#define SSID "??????????"
#define PASSWORD "???????????"

#define DEBUG false 

#define LED_WLAN 13
#define CHECK_TIME_SPAN 60

bool _time_open = true;
int _time_open_hour = 7;
int _time_open_minute = 30;

NanoESP nanoesp = NanoESP();
NanoESP_HTTP http = NanoESP_HTTP(nanoesp); //init http library with an instance of nanoesp

DateTime last_Check_DT;

void setup() {

  //if(DEBUG){
   Serial.begin (19200);
  //}
  
  //RTC
  //_startup_Error += 
  
  setup_Rtc();

  //DOOR
  //_startup_Error += 
  setup_Door();
  
  //webserver  
  nanoesp.init();

  //! Only if you want to change the ACCESPOINT configuration
  //nanoesp.configWifi(ACCESSPOINT, "NanoESP", "");
  
  //! Only if you use a new Network
 // nanoesp.configWifi(DUAL, SSID, PASSWORD);
    
  //Dual Mode
  nanoesp.configWifiMode(DUAL);
  delay(1000);
  
  // Wait till Connected to WiFi
  if (nanoesp.wifiConnected())digitalWrite(LED_WLAN, HIGH); else Serial.println(F("Error WLAN Connect"));

  //Start TCP Server
  if (nanoesp.startTcpServer(80))    Serial.println(F("TCP Server Activ")); else   Serial.println(F("TCP Server Error"));

  //Print IP to Terminal
  //if(DEBUG){
    Serial.println(nanoesp.getIp());
  //}
  last_Check_DT = rtc.now() - TimeSpan(570);

  if(DEBUG){
    //Serial.println(get_DateTime_String(rtc.now()));
    //Serial.println(get_DateTime_String(last_Check_DT));
  }
  
  TimeSpan ts = rtc.now() - last_Check_DT;
  
  if(DEBUG){
    //Serial.println(ts.totalseconds(), DEC);
  }
}

int get_Parameter(String nameParameter, String stringParameter){
  
  String sTemp = stringParameter.substring(stringParameter.indexOf(nameParameter));

  sTemp = sTemp.substring(sTemp.indexOf(nameParameter), sTemp.indexOf("&"));

  sTemp = sTemp.substring(sTemp.indexOf("=") + 1);
  
  return sTemp.toInt();
  }

bool get_Parameter_Checkbox(String nameParameter, String stringParameter){

  if(-1 == stringParameter.indexOf(nameParameter)){
    return false;
  }
  else{
    return true;  
  }
}

String get_FixopenString(){

  String sFixOpen = F("Fix open time: ");
      if(_time_open){
        sFixOpen += F("ON ");
        sFixOpen += (String)_time_open_hour;
        sFixOpen += F(":");
        sFixOpen += (String)_time_open_minute;
      }
      else{
        sFixOpen += F("OFF");
      }
  sFixOpen += F(" <a href=\"/fixoset\">Set</a></h1>");
  return sFixOpen;
  }

void loop() {
  String method, ressource, parameter;
  int id;
  String link_home = F("<h1><a href=\"/\">Home</a><br><br>");

  DateTime now = rtc.now();
  TimeSpan lastCheck_TS = now - last_Check_DT;
  
  if (http.recvRequest(id, method, ressource, parameter)) { //Incoming request, parameters by reference

    if(DEBUG){
      Serial.println("New Request from id :" + String(id) + ", method: " + method +  ", ressource: " + ressource +  " parameter: " + parameter);
    }

    if(ressource == "/"){  //index page
      http.sendFromFlash(id, indexPage, sizeof(indexPage)); //send website from website.h
      nanoesp.closeConnection(id); //close connection so browser knows website transmission is complete
    }
    else if (ressource == "/open") { //open door
      //String doorOpenPage = link_home;
      link_home += open_Door(now.unixtime());
      link_home += F("</h1>");
      nanoesp.sendDataClose(id, link_home);
    }
    else if (ressource == "/close") { //close door
      //String doorClosePage = link_home;
      link_home += close_Door(now.unixtime());
      link_home += F("</h1>");
      nanoesp.sendDataClose(id, link_home);
    }
    else if (ressource == "/sun") { //status page
      link_home += get_SunRise_String(now);
      link_home += F("<br>");
      link_home += get_SunSet_String(now);
      link_home += F("</h1>");
      nanoesp.sendDataClose(id, link_home);
    }
    else if (ressource == "/battery") { //status page
      link_home += get_Voltage_Battery_String();
      link_home += F("</h1>");
      nanoesp.sendDataClose(id, link_home);
    }
    else if (ressource == "/temp") { //status page
      link_home += get_Temperature_String();
      link_home += F("</h1>");
      nanoesp.sendDataClose(id, link_home);
    } 
    else if (ressource == "/door") { //status page
      link_home += F("Door is: ");
      link_home += get_Door_Status_String();
      link_home += F("</h1>");
      nanoesp.sendDataClose(id, link_home);
    }
    else if (ressource == "/fixopen") { //fixopen
      link_home += get_FixopenString();
      nanoesp.sendDataClose(id, link_home);
    }
    else if (ressource == "/fixoset") { //fixopenset
      http.sendFromFlash(id, fixOpenTimeSet, sizeof(fixOpenTimeSet)); //send website from website.h
      nanoesp.closeConnection(id); //close connection so browser knows website transmission is complete
    }
    else if (ressource == "/fixosetdone") { //fixopensetdone
      if(DEBUG){
        Serial.println(get_Parameter_Checkbox("fixo", parameter));
        Serial.println(get_Parameter("hour", parameter));
        Serial.println(get_Parameter("minute", parameter));
      }
      
      _time_open = get_Parameter_Checkbox("fixo", parameter);
      _time_open_hour = get_Parameter("hour", parameter);
      _time_open_minute = get_Parameter("minute", parameter);

      http.sendFromFlash(id, indexPage, sizeof(indexPage)); //send website from website.h
      nanoesp.closeConnection(id); //close connection so browser knows website transmission is complete
      
      //String webPage = link_home;
      //webPage += get_FixopenString();
      //nanoesp.sendDataClose(id, webPage);
    }
    else if (ressource == "/time") { //status page
      link_home += F("Time: ");
      link_home += get_DateTime_String(now);
      link_home += F(" <a href=\"/timeset\">Set</a></h1>");
      nanoesp.sendDataClose(id, link_home);
    }
    else if (ressource == "/test") { //test page
      nanoesp.sendDataClose(id, F("<!DOCTYPE html>\n<html>\n<head>\n<title>Page Title</title>\n</head>\n<body>\n\n<h1>This is a Heading</h1>\n<p>This is a paragraph.</p>\n\n</body>\n</html>\0"));
    }
    else if (ressource == "/timeset") {
      if(DEBUG){
        Serial.println(get_Parameter("year", parameter));
        Serial.println(get_Parameter("month", parameter));
        Serial.println(get_Parameter("minute", parameter));
      }
      http.sendFromFlash(id, timeSet, sizeof(timeSet)); //send website from website.h
      nanoesp.closeConnection(id); //close connection so browser knows website transmission is complete
    }
    else if (ressource == "/timesetdone") {
      
      int py = get_Parameter("year", parameter);
      int pm = get_Parameter("month", parameter);
      int pd = get_Parameter("day", parameter);
      int ph = get_Parameter("hour", parameter);
      int pmin = get_Parameter("minute", parameter);
      
      if(py > 2016 && py < 2030 &&
         pm > 0 && pm < 13 &&
         pd > 0 && pd < 32 &&
         ph >= 0 && ph < 24 &&
         pmin >= 0 && pmin < 60){

            //perform time adjust
            rtc.adjust(DateTime(py, pm, pd, ph, pmin, 0));
            
            DateTime currentDT = rtc.now();
            link_home += F("Ok: Set Time: ");
            link_home += get_DateTime_String(currentDT);
            nanoesp.sendDataClose(id, link_home);
        }
        else{
          nanoesp.sendDataClose(id, link_home + F("Error: Set time failed!"));
          }
          
    }
    else {
      nanoesp.sendDataClose(id, link_home + F("Error: Function not supported!"));
    }
  }

  /*--------------------------------------------------------------------------------------------------------------*/
  
  
  if(now.hour() > 15){ // check for sunset after 15th hour

     
  
  
  if(CHECK_TIME_SPAN < lastCheck_TS.totalseconds()){ //check every CHECK_TIME_SPAN value

      //Serial.println("in close check");
      //Serial.println("Last CLOSE check:" + (String)lastCheck_TS.totalseconds());

      last_Check_DT = now;
      DateTime sunSet_DT(now.year(), now.month(), now.day(), get_Sunset_Hour(now.month(), now.day()), get_Sunset_Minute(now.month(), now.day()),0);
      TimeSpan sunSet_TS = now - sunSet_DT;
      //Serial.println("TS to sunset: " + (String)sunSet_TS.totalseconds());
      if(1200L < sunSet_TS.totalseconds()){ //close door
        //Serial.println("Door will close....");
        close_Door(now.unixtime());
        }
    }
  }else if(now.hour() < 9){ // check for sunrise before 9th hour

    

    if(CHECK_TIME_SPAN < lastCheck_TS.totalseconds()){ //check every CHECK_TIME_SPAN value

      //Serial.println(get_DateTime_String(current_Time_DT));

      //Serial.println("in open check");
      //Serial.println("Last OPEN check:" + (String)lastCheck_TS.totalseconds());
      
       last_Check_DT = now;

       int sunrise_hour = get_Sunrise_Hour(now.month(), now.day());
       int sunrise_minute = get_Sunrise_Minute(now.month(), now.day());

       if(_time_open){

         sunrise_hour = _time_open_hour;
         sunrise_minute = _time_open_minute;
       }
       
       DateTime sunRise_DT(now.year(), now.month(), now.day(), sunrise_hour, sunrise_minute,0);
       TimeSpan sunRise_TS = now - sunRise_DT;
       //Serial.println("TS to sunrise: " + (String)sunRise_TS.totalseconds());
       if(60L < sunRise_TS.totalseconds()){ //open door
          //Serial.println("Door will open....");
          //Serial.println(current_Time_DT.unixtime(), DEC);
          open_Door(now.unixtime());
          }
          //Serial.println("rrr");
    }
  }

  
  
}



