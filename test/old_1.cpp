#include <Arduino.h>

#include <WiFi.h>
#include <update_time.h>

//int pinElectric = GPIO_NUM_35;
int pinElectric = GPIO_NUM_32;
volatile unsigned int impulseCounter = 0;
unsigned long last_pressed =890040;
volatile boolean lastImpuls = 0; 
volatile boolean Impuls = 0; 

void goToSleep(){

Serial.println("Going to sleep now");
//esp_sleep_enable_timer_wakeup(30 * 1000000);
//esp_light_sleep_start();
  //esp_deep_sleep_start();
update_time(5);
delay(30000);
}

hw_timer_t *timer = NULL; 
void IRAM_ATTR onTimer(){
  Serial.println(day()+String(':')+hour()+String(':')+minute()+String(':')+second());
  Serial.println(impulseCounter);
}

void pulse(){
//static unsigned long last_interrupt_time = 0;
//unsigned long interrupt_time = millis();

Impuls =digitalRead(pinElectric);
//delay(5);
  if ( (millis()-last_pressed)>1000 && /*(Impuls  == 1) &&*/ (lastImpuls == 0 )) {
 
 impulseCounter ++; 
  }


//  delay(5);
last_pressed = millis() ;
//lastImpuls = digitalRead(pinElectric);
lastImpuls = Impuls;
//last_interrupt_time = interrupt_time;

}

void setup() {
  Serial.begin(115200);
  pinMode(pinElectric, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pinElectric),pulse, FALLING);

  /*
  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer,1*1000*1000,true);
  timerAlarmEnable(timer);
*/
//update_time(5);



}

void loop()
{
 Serial.println(day()+String(':')+hour()+String(':')+minute()+String(':')+second());
 Serial.println(impulseCounter);
 Serial.println(digitalRead(pinElectric));
 Serial.print("last impuls");
 Serial.println(lastImpuls);

 delay(1000);

// Serial.println(digitalRead(pinElectric));
//Serial.println(analogRead(pinElectric));
//Serial.println(digitalRead(pinElectric));

//delay(1000);
}



