#ifndef _SETUPAP_h
#define _SETUPAP_h

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <setup.h>


#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>

void initParameter(SetUpData &sett, DataAll_IMPLS &all_IMPLS ,CalculatedData &calcdata);
void setup_ap(SetUpData &sett, DataAll_IMPLS &all_IMPLS ,CalculatedData &calcdata);
void saveParamCallback();


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

    WiFiManagerParameter *param_hostname_mqtt; // адрес сервера
    IntParameter *param_port_mqtt; // порт сервера
    WiFiManagerParameter *param_mqtt_user; // логин mqtt брокера
    WiFiManagerParameter *param_mqtt_password; // пароль mqtt брокера

    WiFiManagerParameter *param_description; // описапние устройсва 
    FloatParameter *HOT_start; // начельные покащаниея счетчиуов
    FloatParameter *COLD_start;

    FloatParameter *T1_start;
    FloatParameter *T2_start; //

    LongParameter *param_impuls_per_kW; // импульсов на 1 кВатт
    LongParameter *param_litres_per_imp; // литров на импульс

    IntParameter *wake_every_min; // переиод отпрвки показаний
    IntParameter *param_Timezon; // часовой пояс

    SetUpData *sett_wifi;  
    DataAll_IMPLS data_wifi;  // для совместимости в будущем
    CalculatedData cdata_wifi; //
};
#endif