#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

/*
 //char* mqtt_server = "";

WiFiManager wm;  


void config_devices(){
WiFiManagerParameter custom_text("hot", "hot data", , 40);

wm.addParameter(&custom_text);
//data=custom_text.getValue();
WiFi.begin();
int8_t time_out_conection = 7;
  while ( WiFi.status() != WL_CONNECTED && time_out_conection !=0 ) {
    delay ( 500 );
    Serial.print ( "." );
    time_out_conection --;
    if (time_out_conection == 0){
      wm.autoConnect();
    }
  }

}
*/