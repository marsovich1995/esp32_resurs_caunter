#ifndef _SEND_MQTT_h
#define _SEND_MQTT_h

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h> // В библиотеке PubSubClient.h необходимо увеличить значения MQTT_KEEPALIVE минимум до 120 (можно больше)
#include <setup.h>



 // WiFi
//PubSubClient client(mqttServer, 1883, wifiConnection); // MQTT
//PubSubClient client(sett.hostname_mqtt, 1883, wifiConnection); // MQTT  

//boolean connect(PubSubClient &client, SetUpData &sett);

boolean SendMqtt(CalculatedData &calcdata, SetUpData &sett );

void sendDataMQTT(const char* nameTopic, float &data, PubSubClient &client);

    
 #endif