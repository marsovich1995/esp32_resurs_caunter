#include <Arduino.h> 
//#include <WiFi.h>
#include <send_mqtt.h>
#include <TimeLib.h>
#include <led.h>
#include <setup.h>
#include <setup_up.h>


SetUpData sett;
CalculatedData calcdata;
DataAll_IMPLS all_IMPLS;


enum buttonstate_t : uint8_t { BTN_RELEARED, BTN_CLICK, BTN_LONGCLICK };
QueueHandle_t queue;


static uint8_t pinElectric = GPIO_NUM_35;
static uint8_t pinCold = GPIO_NUM_32;
static uint8_t pinHot = GPIO_NUM_34;
static uint8_t BTN= GPIO_NUM_23;

hw_timer_t *timer = NULL; 

uint64_t timer_count = 0;

void  onTimer(){ // дейсвие по таймеру
static uint64_t timer_count = 1;
  if (timer_count == sett.period_send){
    buttonstate_t state2;
    state2 = BTN_CLICK;
    xQueueSendFromISR(queue, &state2, NULL);
     timer_count = 0;
  }
timer_count ++;  
}

void btn_p(){
  const uint32_t CLICK_TIME = 50;
  const uint32_t LONGCLICK_TIME = 500;
  static uint32_t lastPressed = 0;
  uint32_t time = millis();
  buttonstate_t state2;
  volatile bool btn = digitalRead(BTN) == HIGH;

  if (btn){
    state2 = BTN_RELEARED;
    lastPressed = time;
  }else{
    if (time - lastPressed >= LONGCLICK_TIME) {     
        state2 = BTN_LONGCLICK;
        }
    else if (time - lastPressed >= CLICK_TIME)  {
      state2 = BTN_CLICK ;  
    }
    
    else state2 = BTN_RELEARED;          
    lastPressed = 0;      
  }
  xQueueSendFromISR(queue, &state2, NULL);
  
}

void interup_cold() {
all_IMPLS.caunt_cold.pulse(pinCold,1000);
}

void interup_hot() {
all_IMPLS.caunt_hot.pulse(pinHot,1000);
}

void interup_elec() {
  if(check_celebrat() || weekday() == 1 || weekday() == 7 || hour()==23 || ( hour() <= 6 && minute() <= 59) ){
    all_IMPLS.caunt_elec_T2.pulse(pinElectric,20,LED_ELEC);    
  }else{
    all_IMPLS.caunt_elec_T1.pulse(pinElectric,20,LED_ELEC);
  }
}


void setup() {

  Serial.begin(115200);
  Serial.println("");

  pinMode(pinElectric, INPUT_PULLUP);
  pinMode(pinHot, INPUT_PULLUP); 
  pinMode(pinCold, INPUT_PULLUP);
  pinMode(BTN, INPUT_PULLDOWN);
  pinMode(LED_ELEC, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(pinCold),interup_cold, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinHot),interup_hot, FALLING);  
  attachInterrupt(digitalPinToInterrupt(pinElectric),interup_elec, FALLING); 
  attachInterrupt(digitalPinToInterrupt(BTN),btn_p, CHANGE);

  queue = xQueueCreate(32,sizeof(buttonstate_t));
  

  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer,60000000,true);
  timerAlarmEnable(timer);

  loadConfig(sett);

  xTaskCreate(blink_Error,"Blink", 1024, NULL, 1 , &blink);

}


void calculate_values(SetUpData &sett, DataAll_IMPLS &all_IMPLS ,CalculatedData &calcdata)
{
  calcdata.data_COLD = sett.data_COLD + (all_IMPLS.caunt_cold.impulses / 1000.0) * sett.liters_per_impuls ;
  calcdata.data_HOT  = sett.data_HOT + (all_IMPLS.caunt_hot.impulses / 1000.0) * sett.liters_per_impuls;
  calcdata.data_T1   = sett.data_T1 + (all_IMPLS.caunt_elec_T1.impulses * 1.0 ) / sett.impuls_per_kW;
  calcdata.data_T2   = sett.data_T2 + (all_IMPLS.caunt_elec_T2.impulses * 1.0 ) / sett.impuls_per_kW  ;
}

void loop()
{
     
  if (sett.start_state == ERROR) set_blink(LED_1HZ); //ledmode = LED_ON;
  else set_blink(LED_OFF); //ledmode = LED_1HZ;          

  buttonstate_t state2;
  if (xQueueReceive(queue, &state2, portMAX_DELAY) == pdTRUE  ){
    vTaskSuspend(blink);
    switch (state2){

      case BTN_CLICK:
        //if(!sett.start_state == ERROR){
          set_blink(LED_8HZ);
          Serial.println("CLICK");
          calculate_values(sett,all_IMPLS,calcdata); 
          Serial.println(String(" cold:")+calcdata.data_COLD +String(" hot:")+calcdata.data_HOT + String(" elc1:")+calcdata.data_T1+ String(" elc2:")+calcdata.data_T2);
          if (SendMqtt(calcdata,sett)) Serial.println ("Send Mqtt OK");
          update_time(sett.TimeZone);   
        //}
      break;

      case BTN_LONGCLICK:
        set_blink(LED_ON);
        Serial.println("LONGCLICK");
        setup_ap(sett,all_IMPLS,calcdata);
        
      break;

      }
    vTaskSuspend(blink);
    } 

}




