//#include <Arduino.h>
//#include <WiFi.h>
//#include <WiFiManager.h>
//#include <setup.h>
#include <setup_up.h>

WiFiManager wm;
WiFiCustomParameter wcp;

const char WATERIUS_HANDLER[] PROGMEM = "Получено: \
                                        </br>\
                                            CH0: <a id=\"value0\"></a> имп. \
                                            Counts: <a id=\"count0\"></a>\
                                            State: <a id=\"state0\"></a>\
                                        </br>\
                                            CH1: <a id=\"value1\"></a> имп. \
                                            Counts: <a id=\"count1\"></a>\
                                            State: <a id=\"state1\"></a>\
                                        </br>";

void connect_wl(){
//    LOG_NOTICE("WIF", "Starting Wifi");
    WiFi.begin();
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < ESP_CONNECT_TIMEOUT)
    {
//        LOG_NOTICE("WIF", "Wifi status: " << WiFi.status());
        Serial.print ( "." );
        delay(500);
    };
}

void initParameter(SetUpData &sett, DataAll_IMPLS &all_IMPLS ,CalculatedData &calcdata){

    wcp.sett_wifi = &sett;
    wcp.data_wifi = all_IMPLS;
    wcp.cdata_wifi = calcdata;
    
   // wcp.param_key = new WiFiManagerParameter( "key", "Ключ:",  sett.key, KEY_LEN-1);
    wcp.param_hostname_mqtt = new WiFiManagerParameter( "host", "Адрес сервера:",  sett.hostname_mqtt, HOST_LEN);
    wcp.param_port_mqtt = new IntParameter( "port", "Порт сервера:",  sett.port_mqtt);
    wcp.param_mqtt_user = new WiFiManagerParameter("use3r", "User:", sett.mqtt_user, lOGIN_LEN);
    wcp.param_mqtt_password = new WiFiManagerParameter("pas3sword", "Password:", sett.mqtt_password, lOGIN_LEN);

   // wcp.param_email = new WiFiManagerParameter( "email", "Адрес эл. почты:",  sett.email, EMAIL_LEN-1);
   // wcp.param_email_title = new ЗаголовокWiFiManagerParameter( "title", ":",  sett.email_title, EMAIL_TITLE_LEN-1);
   // wcp.param_email_template = new WiFiManagerParameter( "template", "Тело письма:",  sett.email_template, EMAIL_TEMPLATE_LEN-1);
  //  wcp.param_hostname_json = new WiFiManagerParameter( "hostname_json", "Адрес сервера для JSON:",  sett.hostname_json, EMAIL_TITLE_LEN-1);
  //?  wcp.param_name = new WiFiManagerParameter( "Name", "Имя устройства:", sett.name, HOSTNAME_LEN-1);
    wcp.HOT_start = new FloatParameter( "channel0", "Вход ГВС (м3):",  sett.data_HOT);
    wcp.COLD_start = new FloatParameter( "channel1", "Вход ХВС (м3):",  sett.data_COLD);

    wcp.T1_start = new FloatParameter("channel_T1", "Вход T1 (кВатт):", sett.data_T1);
    wcp.T2_start = new FloatParameter("channel_T2", "Вход T2 (кВатт):", sett.data_T2);
  //?  wcp.param_channel0_start = new FloatParameter( "channel0", "Вход 0 ГВС (м3):",  cdata.channel0);
  //  wcp.param_sn0 = new WiFiManagerParameter( "sn0", "Серийный номер:",  sett.sn0, SN_LEN-1);
  //?  wcp.param_channel1_start = new FloatParameter( "channel1", "Вход 1 ХВС (м3):",  cdata.channel1);
  //  wcp.param_sn1 = new WiFiManagerParameter( "sn1", "Серийный номер:",  sett.sn1, SN_LEN-1);
    wcp.param_litres_per_imp = new LongParameter( "factor1", "Литров на импульс:",  sett.liters_per_impuls);
    wcp.param_impuls_per_kW = new LongParameter( "factor2", "Импульсов на кВатт:",  sett.impuls_per_kW);
    wcp.wake_every_min = new IntParameter( "wake_every_min", "Период отправки, мин:",  sett.period_send);
    wcp.param_Timezon = new IntParameter( "TimeZone", "Часовой пояс, ч:",  sett.TimeZone);
    wcp.param_description = new WiFiManagerParameter( "Description", "Описание:", sett.description, DESCRIP_LEN);

}


void setup_ap(SetUpData &sett, DataAll_IMPLS &all_IMPLS ,CalculatedData &calcdata){
    std::vector<const char *> menu = {"param","sep", "wifi","sep","exit"};
    wm.setMenu(menu);
    wm.setClass("invert");
    initParameter(sett, all_IMPLS, calcdata);
    WiFiManagerParameter custom_html(WATERIUS_HANDLER);
  //  WiFiManagerParameter javascript_callback(WATERIUS_CALLBACK);

  //  wm.setWebServerCallback(bindServerCallback);
    wm.addParameter(&custom_html);
  //  wm.addParameter(&javascript_callback);

//    wm.addParameter( wcp.param_name );
    WiFiManagerParameter counters_text("<p>Счетчики:</p>");
    wm.addParameter( &counters_text );
    wm.addParameter( wcp.HOT_start );
    wm.addParameter( wcp.COLD_start );
    wm.addParameter(wcp.T1_start);
    wm.addParameter(wcp.T2_start);

    wm.addParameter( wcp.param_litres_per_imp );
    wm.addParameter( wcp.param_impuls_per_kW);

    WiFiManagerParameter mqtt_text("<p>MQTT:</p>");
    wm.addParameter( &mqtt_text );
    wm.addParameter( wcp.param_hostname_mqtt); //добавить порт, логин пароль
    wm.addParameter( wcp.param_port_mqtt);
    wm.addParameter( wcp.param_mqtt_user);
    wm.addParameter( wcp.param_mqtt_password);

    wm.addParameter( wcp.wake_every_min );
    wm.addParameter( wcp.param_description );
    wm.addParameter(wcp.param_Timezon);

    wm.setSaveParamsCallback(saveParamCallback);

    wm.setConfigPortalTimeout(300);
    wm.setConnectTimeout(15);
    wm.startConfigPortal( "myESP" );   
  //  wm.resetSettings();
  Serial.println("Restrart");
  ESP.restart();

}
void saveParamCallback(){
 
  strncpy(wcp.sett_wifi->hostname_mqtt, wcp.param_hostname_mqtt->getValue(),HOST_LEN);
  wcp.sett_wifi->port_mqtt = wcp.param_port_mqtt->getValue();
  strncpy(wcp.sett_wifi->mqtt_user, wcp.param_mqtt_user->getValue(),lOGIN_LEN);
  strncpy(wcp.sett_wifi->mqtt_password, wcp.param_mqtt_password->getValue(),lOGIN_LEN);

  wcp.sett_wifi->data_COLD = wcp.COLD_start->getValue();
  wcp.sett_wifi->data_HOT = wcp.HOT_start->getValue();
  wcp.sett_wifi->data_T1 = wcp.T1_start->getValue();
  wcp.sett_wifi->data_T2 = wcp.T2_start->getValue();

  wcp.sett_wifi->liters_per_impuls = wcp.param_litres_per_imp->getValue();
  wcp.sett_wifi->impuls_per_kW = wcp.param_impuls_per_kW->getValue();

  wcp.sett_wifi->period_send = wcp.wake_every_min->getValue();
  strncpy(wcp.sett_wifi->description, wcp.param_description->getValue(),DESCRIP_LEN);
  wcp.sett_wifi->TimeZone = wcp.param_Timezon->getValue();
  //wcp.sett_wifi->target_restart = 1;
  wcp.sett_wifi->start_state = SAVED;
  storeConfig(*wcp.sett_wifi);

}

void stop_ap(){
  
    wm.stopConfigPortal();
   
//    LOG_NOTICE( "AP", "stop config portal" );
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
