#include <Arduino.h>
#include <WiFi.h>
#include <update_time.h>
#include <send_mqtt.h>
#include <TimeLib.h>
#include <setup.h>
#include <setup_up.h>

WiFiManager wm;
char mqtt_server[40];
WiFiManagerParameter custom_text("hot", "hot data", mqtt_server, 40);



SetUpData sett;
CalculatedData calcdata;
DataAll_IMPLS all_IMPLS;


struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};


static uint8_t pinElectric = GPIO_NUM_35;
static uint8_t pinCold = GPIO_NUM_32;
static uint8_t pinHot = GPIO_NUM_34;
static uint8_t btn = GPIO_NUM_23;
Button button1 = {btn, 0, false};
hw_timer_t *timer = NULL; 
volatile boolean timerFlag = 0;
void IRAM_ATTR onTimer(){ // дейсвие по таймеру
timerFlag = true;
}
/*
void IRAM_ATTR pulse(){
Impuls =digitalRead(pinHot);
  if ( (millis()-last_Timp_H)>1000 &&  (lastImpuls_H == 0 )) {
 Display_Hot  += 10;
  }
last_Timp_H = millis();
lastImpuls_H = Impuls;
}


void IRAM_ATTR pulse_H(){
Impuls =digitalRead(pinHot);
  if ( (millis()-last_Timp_H)>1000 &&  (lastImpuls_H == 0 )) {
 Display_Hot  += 10;
  }
last_Timp_H = millis();
lastImpuls_H = Impuls;
}

void IRAM_ATTR pulse_C(){
Impuls =digitalRead(pinCold);
  if ( (millis()-last_Timp_C)>1000 &&  (lastImpuls_C == 0 )) {
 Display_Cold  += 10;
  }
last_Timp_C = millis();
lastImpuls_C = Impuls;
}

void IRAM_ATTR pulse_E(){
Impuls =digitalRead(pinElectric);
  if ( (millis()-last_Timp_E)>20 &&  (lastImpuls_E == 0 )) {
 Display_E1 ++;
  }
last_Timp_E = millis();
lastImpuls_E = Impuls;
}
*/


void IRAM_ATTR btn_p(){
  button1.numberKeyPresses ++;
  button1.pressed = true;
}

void interup_cold() {
all_IMPLS.caunt_cold.pulse(pinCold,1000);
}

void interup_hot() {
all_IMPLS.caunt_hot.pulse(pinHot,1000);
}

void interup_elec() {
  if(check_celebrat() || weekday() == 1 || weekday() == 7 || hour()==23 || ( hour() <= 6 && minute() <= 59) ){
    all_IMPLS.caunt_elec_T2.pulse(pinElectric,20);    
  }else{
    all_IMPLS.caunt_elec_T1.pulse(pinElectric,20);
  }
}




void setup() {

sett = {5,5,5,5};
sett.impuls_per_kW = 1000;
sett.liters_per_impuls = 10;

  Serial.begin(115200);
  pinMode(pinElectric, INPUT_PULLUP);
  pinMode(pinHot, INPUT_PULLUP); 
  pinMode(pinCold, INPUT_PULLUP);
  pinMode(btn, INPUT_PULLDOWN);

  attachInterrupt(digitalPinToInterrupt(pinCold),interup_cold, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinHot),interup_hot, FALLING);  
  attachInterrupt(digitalPinToInterrupt(pinElectric),interup_elec, FALLING);
  attachInterrupt(digitalPinToInterrupt(btn),btn_p, FALLING);
  
  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer,3600000000,true);
  timerAlarmEnable(timer);

//config_devices();
//update_time(5);

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

//wcp.startConfigPortal("Onellas");
//setTime(1604991480);

}


void calculate_values(const SetUpData &sett, DataAll_IMPLS &all_IMPLS ,CalculatedData &calcdata)
{
  calcdata.data_COLD = sett.data_COLD + (all_IMPLS.caunt_cold.impulses / 1000.0) * sett.liters_per_impuls ;
  calcdata.data_HOT  = sett.data_HOT + (all_IMPLS.caunt_hot.impulses / 1000.0) * sett.liters_per_impuls;
  calcdata.data_T1   = sett.data_T1 + (all_IMPLS.caunt_elec_T1.impulses * 1.0 ) / sett.impuls_per_kW;
  calcdata.data_T2   = sett.data_T2 + (all_IMPLS.caunt_elec_T2.impulses * 1.0 ) / sett.impuls_per_kW  ;
}

void loop()
{
calculate_values(sett,all_IMPLS,calcdata); 
Serial.println(String(" cold:")+calcdata.data_COLD +String(" hot:")+calcdata.data_HOT + String(" elc1:")+calcdata.data_T1+ String(" elc2:")+calcdata.data_T2);
//Serial.println(String(" caunt2: ")+all_IMPLS.caunt_elec_T2.impulses);
//Serial.println(String(" cold:")+digitalRead(pinCold) +String(" hot:")+ digitalRead(pinHot) + String(" elc:")+ digitalRead(pinElectric));

delay(5000);

    Serial.println(day()+String('.')+ month() +String('.')+ year() +String(' ')+hour()+String(':')+minute());
    Serial.println(String(mqtt_server));
 /* 
if (timerFlag){
  calculate_values(sett,all_IMPLS,calcdata);
  SendMqtt(calcdata);
  update_time(5);
  timerFlag = 0;
}
*/


if (button1.pressed == 1){
   wm.startConfigPortal("OnDemandAP");
   strcpy(mqtt_server, custom_text.getValue());
  // mydata2 = custom_text.getValue();
  button1.pressed = 0;
}


}




