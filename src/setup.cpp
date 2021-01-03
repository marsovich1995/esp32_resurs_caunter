
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

   // if (sett.target_restart == 1){
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



/*
    if (old_crc == new_crc)
    {
        LOG_NOTICE("CFG", "CRC ok");

        // Для безопасной работы с буферами,  в библиотеках может не быть нет проверок
        sett.key[KEY_LEN-1] = '\0';
        sett.hostname[HOSTNAME_LEN-1] = '\0';
        sett.email[EMAIL_LEN-1] = '\0';
        sett.email_title[EMAIL_TITLE_LEN-1] = '\0';
        sett.email_template[EMAIL_TEMPLATE_LEN-1] = '\0'; 
        sett.hostname_json[EMAIL_TITLE_LEN-1] = '\0';
        sett.name[HOSTNAME_LEN-1] = '\0';
        sett.sn0[SN_LEN-1] = '\0';
        sett.sn1[SN_LEN-1] = '\0';
        sett.description[EMAIL_TITLE_LEN - 1] = '\0';
        sett.coap_hostname[HOSTNAME_LEN-1] = '\0';
     

        
        return true;
    }
    else 
    {    // Конфигурация не была сохранена в EEPROM, инициализируем с нуля
        LOG_WARNING( "CFG", "crc failed=" << sett.crc_cfg );

        // Заполняем нулями всю конфигурацию
        memset(&sett, 0, sizeof(sett));

        sett.version = CURRENT_VERSION;  //для совместимости в будущем
        sett.liters_per_impuls = LITRES_PER_IMPULS_DEFAULT;
        
        String hostname = BLYNK_DEFAULT_DOMAIN;
        strncpy0(sett.hostname, hostname.c_str(), HOSTNAME_LEN);

        String email_title = "Новые показания {DEVICE_NAME}";
        strncpy0(sett.email_title, email_title.c_str(), EMAIL_TITLE_LEN);

        String email_template = "Горячая: {V0}м3, Холодная: {V1}м3<br>За день:<br>Горячая: +{V3}л, Холодная: +{V4}л<br>Напряжение:{V2}В";
        strncpy0(sett.email_template, email_template.c_str(), EMAIL_TEMPLATE_LEN);

        String hostname_json = "http://myhome.ru:5005";
        strncpy0(sett.hostname_json, hostname_json.c_str(), EMAIL_TITLE_LEN);

        strncpy0(sett.coap_hostname, "coap://myhome.ru:5005", HOSTNAME_LEN);

        String name = "Waterius_";
        strncpy0(sett.name, name.c_str(), HOSTNAME_LEN);

        strncpy0(sett.sn0, String("0000000").c_str(), SN_LEN);
        strncpy0(sett.sn1, String("0000000").c_str(), SN_LEN);

        sett.impulses0_previous = 0;
        sett.impulses1_previous = 0;
        sett.watersensor_previous = false;

        sett.wake_every_min = 60;
        sett.sensors = 0;

        //caclulace CRC for config
        sett.crc_cfg = 0;
        uint8_t new_crc = crc_8((unsigned char*)&sett, sizeof(sett));
        LOG_NOTICE( "CFG", "CRC calculated=" << new_crc << ", CRC Stored=" << sett.crc_cfg);
        sett.crc_cfg = new_crc;
        
        LOG_NOTICE("CFG", "version=" << sett.version << ", hostname=" << hostname);
        return false;
    }
}*/
