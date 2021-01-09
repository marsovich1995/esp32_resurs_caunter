
#include <setup.h>
#include <setup_up.h>

extern int8_t case_status;

boolean check_celebrat(){
  for (uint8_t i=0; i<14; i++){
      if (celebration[i][0]== day() && celebration[i][1] == month() /*&& year() > 2019 */){
        return true;
      }
  }
  return false;
}



void storeConfig( SetUpData &sett)
{
    EEPROM.begin( sizeof(sett) );
    EEPROM.put(0, sett);
   
    if (EEPROM.commit())
    {
        Serial.println("CFG Config stored OK");    
    }
    else
    {
        Serial.println("CFG Config stored FAILED");
    }
}




void loadConfig( SetUpData &sett) 
{
   
    EEPROM.begin( sizeof(sett) );
    EEPROM.get(0, sett);
    if (sett.first_start == !50){
        memset(&sett, 0, sizeof(sett));
        String hostname_mqtt = "myhome.ru";
        strncpy(sett.hostname_mqtt, hostname_mqtt.c_str(), HOST_LEN);
        sett.port_mqtt = 1883;
        String login_mqtt = "mqtt";
        strncpy(sett.mqtt_user, login_mqtt.c_str(), lOGIN_LEN);
        strncpy(sett.mqtt_password, login_mqtt.c_str(), lOGIN_LEN);

        sett.liters_per_impuls = 10;
        sett.impuls_per_kW = 1000;
        sett.period_send = 60;
        sett.first_start=50;
        sett.TimeZone = 5;
        storeConfig(sett);
    }

    if (sett.start_state == SAVED){

        sett.start_state = GOOD;
        storeConfig(sett);
        if(!update_time(sett.TimeZone)){
            Serial.println("Время не утановлено");
            sett.start_state = ERROR;
        }; // добавить настройку

    }else{
        sett.data_HOT = 0;
        sett.data_COLD = 0;
        sett.data_T1 = 0;
        sett.data_T2 = 0;
        sett.start_state = ERROR;
     }
}

boolean update_time( int Timezone ){
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP); // Инициализация

  connect_wl();

  timeClient.begin(); 
  if(!timeClient.update()) return false;
  time_t t = timeClient.getEpochTime();

  setTime(t); 
  adjustTime(Timezone * 3600); // Внести поправку Часового пояса
  
  WiFi.disconnect(); // отключится
  Serial.println("Время уставновлено");
  return true;
}


void connect_wl(){
    WiFi.begin();
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < ESP_CONNECT_TIMEOUT)
    {
        Serial.print ( "." );
        delay(500);
    };
}

void disconnect_wl(){
    Serial.println("Disconnecting Wifi");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
}