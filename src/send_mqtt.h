#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h> // В библиотеке PubSubClient.h необходимо увеличить значения MQTT_KEEPALIVE минимум до 120 (можно больше)
#include <setup.h>
IPAddress   mqttServer(192, 168, 1, 8); // IP-адрес MQTT-брокера
const char* clientName = "esp32"; // Имя клиента
const char* mqttUser = "mqtt"; // Имя пользователя для MQTT-брокера
const char* mqttPassword = "mqtt"; // Пароль для MQTT-брокера
const char* mqttTopicCold = "sensors/water/cold"; // Топик для получения данных
const char* mqttTopicHot =  "sensors/water/hot"; // Топик для получения данных
const char* mqttTopicElectricT1 = "sensors/water/electricT1"; // Топик для получения данных
const char* mqttTopicElectricT2 = "sensors/water/electricT2"; // Топик для получения данных

WiFiClient wifiConnection; // WiFi
PubSubClient client(mqttServer, 1883, wifiConnection); // MQTT 

void connect()
{
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
  while (!client.connect(clientName, mqttUser, mqttPassword)) {
    delay(50);
  }
}
void sendDataMQTT(const char* nameTopic, float &data);

void SendMqtt(CalculatedData &calcdata)
{

WiFi.begin();
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

    connect();
    sendDataMQTT(mqttTopicCold, calcdata.data_COLD);
    sendDataMQTT(mqttTopicHot, calcdata.data_HOT);
    sendDataMQTT(mqttTopicElectricT1, calcdata.data_T1);
    sendDataMQTT(mqttTopicElectricT2, calcdata.data_T2);
 }

void sendDataMQTT(const char* nameTopic, float &data){
//char intToSting[5];
//itoa(data, intToSting, 10); 
client.publish(nameTopic, String((float)data,3).c_str());
delay(50);
 }
    
 