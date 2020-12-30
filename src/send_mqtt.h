#ifndef _SEND_MQTT_h
#define _SEND_MQTT_h

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h> // В библиотеке PubSubClient.h необходимо увеличить значения MQTT_KEEPALIVE минимум до 120 (можно больше)
#include <setup.h>

//IPAddress   mqttServer(192, 168, 1, 8); // IP-адрес MQTT-брокера
const char* clientName = "esp32"; // Имя клиента
//const char* mqttUser = "mqtt"; // Имя пользователя для MQTT-брокера
//const char* mqttPassword = "mqtt"; // Пароль для MQTT-брокера
const char* mqttTopicCold = "sensors/water/cold"; // Топик для получения данных
const char* mqttTopicHot =  "sensors/water/hot"; // Топик для получения данных
const char* mqttTopicElectricT1 = "sensors/water/electricT1"; // Топик для получения данных
const char* mqttTopicElectricT2 = "sensors/water/electricT2"; // Топик для получения данных

WiFiClient wifiConnection; // WiFi
//PubSubClient client(mqttServer, 1883, wifiConnection); // MQTT
//PubSubClient client(sett.hostname_mqtt, 1883, wifiConnection); // MQTT  

void connect(PubSubClient &client, SetUpData &sett)
{
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
  while (!client.connect(clientName, sett.mqtt_user, sett.mqtt_password)) {
    delay(50);
  }
}
void sendDataMQTT(const char* nameTopic, float &data, PubSubClient &client);

void SendMqtt(CalculatedData &calcdata, SetUpData &sett )
{
PubSubClient client(sett.hostname_mqtt, sett.port_mqtt, wifiConnection); // MQTT   

WiFi.begin();
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

    connect(client, sett);
    sendDataMQTT(mqttTopicCold, calcdata.data_COLD, client);
    sendDataMQTT(mqttTopicHot, calcdata.data_HOT, client);
    sendDataMQTT(mqttTopicElectricT1, calcdata.data_T1, client);
    sendDataMQTT(mqttTopicElectricT2, calcdata.data_T2, client);
 }

void sendDataMQTT(const char* nameTopic, float &data, PubSubClient &client){
//char intToSting[5];
//itoa(data, intToSting, 10); 
client.publish(nameTopic, String((float)data,3).c_str());
delay(50);
 }
    
 #endif