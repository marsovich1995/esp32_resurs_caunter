#ifndef UDDATE_TIME_H
#define UDDATE_TIME_H

#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <WiFi.h>
#include <setup_up.h>
void update_time( int Timezone ){

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP); // Инициализация

//WiFi.begin(ssid, password); // Подключиться к сети
/*
WiFi.begin();
  while ( WiFi.status() != WL_CONNECTED  ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println(WiFi.localIP());
*/
connect_wl();
timeClient.begin(); 
timeClient.update();
time_t t = timeClient.getEpochTime();

setTime(t); 
adjustTime(Timezone * 3600); // Внести поправку Часового пояса
 
WiFi.disconnect(); // отключится
Serial.println("Время уставновлено");

delay ( 5000 );
}

#endif