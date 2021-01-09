
#include <send_mqtt.h>

//IPAddress   mqttServer(192, 168, 1, 8); // IP-адрес MQTT-брокера
const char* clientName = "esp32"; // Имя клиента
//const char* mqttUser = "mqtt"; // Имя пользователя для MQTT-брокера
//const char* mqttPassword = "mqtt"; // Пароль для MQTT-брокера
const char* mqttTopicCold = "sensors/water/cold"; // Топик для получения данных
const char* mqttTopicHot =  "sensors/water/hot"; // Топик для получения данных
const char* mqttTopicElectricT1 = "sensors/water/electricT1"; // Топик для получения данных
const char* mqttTopicElectricT2 = "sensors/water/electricT2"; // Топик для получения данных
WiFiClient wifiConnection;

void  sendDataMQTT(const char* nameTopic, float &data, PubSubClient &client){
client.publish(nameTopic, String((float)data,3).c_str());
delay(50);
}

boolean SendMqtt(CalculatedData &calcdata, SetUpData &sett ){

    PubSubClient client(sett.hostname_mqtt, sett.port_mqtt, wifiConnection); // MQTT   
    connect_wl();
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    
      if (client.connect(clientName, sett.mqtt_user, sett.mqtt_password)){
      sendDataMQTT(mqttTopicCold, calcdata.data_COLD, client);
      sendDataMQTT(mqttTopicHot, calcdata.data_HOT, client);
      sendDataMQTT(mqttTopicElectricT1, calcdata.data_T1, client);
      sendDataMQTT(mqttTopicElectricT2, calcdata.data_T2, client);
      disconnect_wl();
      return true;
      }else{
        Serial.println("Connect error");
      }
disconnect_wl();
return false;
}

// boolean connect(PubSubClient &client, SetUpData &sett)
// {
//     uint32_t start = millis();
//   // while (!client.connect(clientName, sett.mqtt_user, sett.mqtt_password)) {
//   //   //if ((millis() - start < 5000)<=0) return false;
//   //   delay(50);
//   // }

// return false;
// }