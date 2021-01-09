//#include <Arduino.h>
//#include <WiFi.h>
//#include <WiFiManager.h>
//#include <setup.h>
#include <setup_up.h>

WiFiManager wm;
WiFiCustomParameter wcp;

void initParameter(SetUpData &sett, DataAll_IMPLS &all_IMPLS ,CalculatedData &calcdata){

    wcp.sett_wifi = &sett;
    wcp.data_wifi = all_IMPLS;
    wcp.cdata_wifi = calcdata;
    
    wcp.param_hostname_mqtt = new WiFiManagerParameter( "host", "Адрес сервера:",  sett.hostname_mqtt, HOST_LEN);
    wcp.param_port_mqtt = new IntParameter( "port", "Порт сервера:",  sett.port_mqtt);
    wcp.param_mqtt_user = new WiFiManagerParameter("use3r", "User:", sett.mqtt_user, lOGIN_LEN);
    wcp.param_mqtt_password = new WiFiManagerParameter("pas3sword", "Password:", sett.mqtt_password, lOGIN_LEN);

    wcp.HOT_start = new FloatParameter( "channel0", "Вход ГВС (м3):",  sett.data_HOT);
    wcp.COLD_start = new FloatParameter( "channel1", "Вход ХВС (м3):",  sett.data_COLD);

    wcp.T1_start = new FloatParameter("channel_T1", "Вход T1 (кВатт):", sett.data_T1);
    wcp.T2_start = new FloatParameter("channel_T2", "Вход T2 (кВатт):", sett.data_T2);

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
  wcp.sett_wifi->start_state = SAVED;
  storeConfig(*wcp.sett_wifi);

}




