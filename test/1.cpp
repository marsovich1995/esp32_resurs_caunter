
#include <PubSubClient.h> // В библиотеке PubSubClient.h необходимо увеличить значения MQTT_KEEPALIVE минимум до 120 (можно больше)

// ОСНОВНЫЕ НАСТРОЙКИ



// КОНТАКТ ДАТЧИКА
int pinNumberTEMT6000 = A0;

// НАСТРОЙКИ

unsigned int impulseCounter = 0;
boolean impulseDetected = false;




// Инициализация
void setup() {
  pinMode(pinNumberTEMT6000, INPUT);

}


void loop()
{


  int illuminanceValue = digitalRead(pinNumberTEMT6000);
  delay(5);

  if ((illuminanceValue == 1) & (impulseDetected == false)) {
    impulseDetected = true;
    delay(5);
  }

  if ((illuminanceValue == 1) & (impulseDetected == true)) {
    impulseDetected = false;
    impulseCounter ++;
    delay(5);
  }



}