#ifndef _SETUPAP_h
#define _SETUPAP_h

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <setup.h>


#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>


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
void connect_wl();
void initParameter(SetUpData &sett, DataAll_IMPLS &all_IMPLS ,CalculatedData &calcdata);
void setup_ap(SetUpData &sett, DataAll_IMPLS &all_IMPLS ,CalculatedData &calcdata);
void saveParamCallback();
void stop_ap();
boolean update_time( int Timezone );


class LongParameter : public WiFiManagerParameter {
public:
    LongParameter():WiFiManagerParameter()
    {}
    LongParameter(const char *id, const char *placeholder, long value, const uint8_t length = 10)
        : WiFiManagerParameter("") {
        init(id, placeholder, String(value).c_str(), length, "", WFM_LABEL_BEFORE);
    }
    long getValue() {
        return String(WiFiManagerParameter::getValue()).toInt();
    }
};

class FloatParameter : public WiFiManagerParameter {
public:
    FloatParameter(): WiFiManagerParameter()
    {}
    FloatParameter(const char *id, const char *placeholder, float value, const uint8_t length = 10)
        : WiFiManagerParameter("") {
        init(id, placeholder, String(value).c_str(), length, "", WFM_LABEL_BEFORE);
    }

    float getValue() {
        return String(WiFiManagerParameter::getValue()).toFloat();
    }
};

class IntParameter : public WiFiManagerParameter {
public:
    IntParameter():WiFiManagerParameter()
    {}
    IntParameter(const char *id, const char *placeholder, long value, const uint8_t length = 5)
        : WiFiManagerParameter("") {
        init(id, placeholder, String(value).c_str(), length, "", WFM_LABEL_BEFORE);
    }
    long getValue() {
        return String(WiFiManagerParameter::getValue()).toInt();
    }
};

struct WiFiCustomParameter{
 //   WiFiManagerParameter *param_key;
    WiFiManagerParameter *param_hostname_mqtt;
    IntParameter *param_port_mqtt;
    WiFiManagerParameter *param_mqtt_user;
    WiFiManagerParameter *param_mqtt_password;
//    WiFiManagerParameter *param_email;
 //   WiFiManagerParameter *param_email_title;
 //   WiFiManagerParameter *param_email_template;
 //   WiFiManagerParameter *param_hostname_json;
  //  WiFiManagerParameter *param_name;
    WiFiManagerParameter *param_description;
    FloatParameter *HOT_start;
    FloatParameter *COLD_start;

    FloatParameter *T1_start;
    FloatParameter *T2_start;

    LongParameter *param_impuls_per_kW;
    LongParameter *param_litres_per_imp;


    IntParameter *wake_every_min;
    IntParameter *param_Timezon;
 //   WiFiManagerParameter *param_sn0;
 //   WiFiManagerParameter *param_sn1;
 //   WiFiManagerParameter *param_coap_hostname;

    SetUpData *sett_wifi;
    DataAll_IMPLS data_wifi;
    CalculatedData cdata_wifi;
};
#endif