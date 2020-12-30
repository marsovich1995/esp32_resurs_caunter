#include <Arduino.h> 
#include <WiFi.h>
//#include <update_time.h>
#include <send_mqtt.h>
#include <TimeLib.h>
#include <led.h>
#include <setup.h>
//  #include <WiFiManager.h>
#include <setup_up.h>

// WiFiManager wm;
// char mqtt_server[40];
// WiFiManagerParameter custom_text("hot", "hot data", mqtt_server, 40);

//sett.period_send=60;


SetUpData sett;
CalculatedData calcdata;
DataAll_IMPLS all_IMPLS;

int8_t case_status;

//enum  { MANUAL, BTN_CLICK, BTN_LONGCLICK,SEND, SETT_LED } state2;
enum buttonstate_t : uint8_t { MANUAL, BTN_CLICK, BTN_LONGCLICK,SEND, SETT_LED };
QueueHandle_t queue;

// struct Button {
//   const uint8_t PIN;
//   uint32_t numberKeyPresses;
//   bool pressed;
// };


static uint8_t pinElectric = GPIO_NUM_35;
static uint8_t pinCold = GPIO_NUM_32;
static uint8_t pinHot = GPIO_NUM_34;
static uint8_t BTN= GPIO_NUM_23;
static uint8_t LED = GPIO_NUM_2;

// Button button1 = {BTN, 0, false};
hw_timer_t *timer = NULL; 
volatile boolean timerFlag = 0;

uint64_t timer_count = 0;

void IRAM_ATTR onTimer(){ // дейсвие по таймеру
static uint64_t timer_count = 1;
  if (timer_count == sett.period_send){
    buttonstate_t state2;
    state2 = SEND;
    xQueueSendFromISR(queue, &state2, NULL);
     timer_count = 0;
  }
timer_count ++;  
//timerFlag = true;
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
//enum buttonstate_t : uint8_t {BTN_RELEASED, BTN_PRESSED, BTN_CLICK, BTN_LONGCLICK};


//buttonstate_t state;
//enum buttonstate_t : uint8_t {BTN_CLICK, BTN_LONGCLICK};


void IRAM_ATTR btn_p(){
  const uint32_t CLICK_TIME = 50;
  const uint32_t LONGCLICK_TIME = 500;
 
  static uint32_t lastPressed = 0;
  uint32_t time = millis();
  buttonstate_t state2;
  volatile bool btn = digitalRead(BTN) == HIGH;

  if (btn){
    state2 = MANUAL;
    lastPressed = time;
  }else{
    if (time - lastPressed >= LONGCLICK_TIME)      state2 = BTN_LONGCLICK;
    else if (time - lastPressed >= CLICK_TIME)  {
      state2 = BTN_CLICK ;  //state = BTN_CLICK;
    //  button1.numberKeyPresses ++;
    }
    
    else state2 = MANUAL;          
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
    all_IMPLS.caunt_elec_T2.pulse(pinElectric,20);    
  }else{
    all_IMPLS.caunt_elec_T1.pulse(pinElectric,20);
  }
}




void setup() {

// sett = {5,5,5,5};
// sett.impuls_per_kW = 1000;
// sett.liters_per_impuls = 10;

  Serial.begin(115200);
  pinMode(pinElectric, INPUT_PULLUP);
  pinMode(pinHot, INPUT_PULLUP); 
  pinMode(pinCold, INPUT_PULLUP);
  pinMode(BTN, INPUT_PULLDOWN);
  pinMode(LED, OUTPUT);


  attachInterrupt(digitalPinToInterrupt(pinCold),interup_cold, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinHot),interup_hot, FALLING);  
  attachInterrupt(digitalPinToInterrupt(pinElectric),interup_elec, FALLING); 
  attachInterrupt(digitalPinToInterrupt(BTN),btn_p, CHANGE);

  queue = xQueueCreate(32,sizeof(buttonstate_t));
  

timer = timerBegin(0,80,true);
timerAttachInterrupt(timer, &onTimer, true);
timerAlarmWrite(timer,60000000,true);
timerAlarmEnable(timer);
 //timerAlarmEnable(timer);


// wm.addParameter(&custom_text); 
///update_time(5);
/*
wm.addParameter(&custom_text); 
wm.addParameter(&custom_factor);
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
*/
//wcp.startConfigPortal("Onellas");
//setTime(1604991480);
if (!loadConfig(sett)){ 
    buttonstate_t state2;
    state2 = SETT_LED;
    xQueueSendFromISR(queue, &state2, NULL);
}

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

buttonstate_t state2;

if (xQueueReceive(queue, &state2, portMAX_DELAY) == pdTRUE  ){
//if (xQueueReceive(queue, &state, pdMS_TO_TICKS(1000)) == pdTRUE){
  switch (state2)
  {
  case BTN_CLICK:
    LEDBlink(LED,1,200);
    Serial.print("CLICK");
    //Serial.println(button1.numberKeyPresses);
    calculate_values(sett,all_IMPLS,calcdata);
    SendMqtt(calcdata,sett);
   break;
  case BTN_LONGCLICK:
    LEDBlink(LED,2,200);
    Serial.println("LONGCLICK");
    setup_ap(sett,all_IMPLS,calcdata);
  break;
  case SEND:
    Serial.println(day()+String('.')+ month() +String('.')+ year() +String(' ')+hour()+String(':')+minute());
  break;  
  case SETT_LED:   
    Serial.println(String("требуется натройка"));
    LEDBlink(LED,1,1000);
    state2 = SETT_LED;
    xQueueSendFromISR(queue, &state2, NULL);
  break; 

  }
  
//  state2= MANUAL;
} 

 
  /*
calculate_values(sett,all_IMPLS,calcdata); 
Serial.println(String(" cold:")+calcdata.data_COLD +String(" hot:")+calcdata.data_HOT + String(" elc1:")+calcdata.data_T1+ String(" elc2:")+calcdata.data_T2);
//Serial.println(String(" caunt2: ")+all_IMPLS.caunt_elec_T2.impulses);
//Serial.println(String(" cold:")+digitalRead(pinCold) +String(" hot:")+ digitalRead(pinHot) + String(" elc:")+ digitalRead(pinElectric));



    Serial.println(day()+String('.')+ month() +String('.')+ year() +String(' ')+hour()+String(':')+minute());
    Serial.println(String(sett.hostname_mqtt)+String("|"));
delay(5000);    
*/
  //  Serial.println(String(mqtt_server));
  
// if (timerFlag){
//   calculate_values(sett,all_IMPLS,calcdata);
//   SendMqtt(calcdata);
//   update_time(5);
//   timerFlag = 0;
// }



//  if (button1.pressed == 1){
//    Serial.print("CLICK");
//    Serial.println(button1.numberKeyPresses);
// //   case_status = 2;
// // //setup_ap(sett,all_IMPLS,calcdata);
// // //calculate_values(sett,all_IMPLS,calcdata); 
// // //Serial.println(String(" cold:")+calcdata.data_COLD +String(" hot:")+calcdata.data_HOT + String(" elc1:")+calcdata.data_T1+ String(" elc2:")+calcdata.data_T2);
// //  //  wm.startConfigPortal("OnDemandAP");
// //   // strcpy(mqtt_server, custom_text.getValue());
// //   // factor = custom_factor.getValue();
// //   // mydata2 = custom_text.getValue();
//    button1.pressed = 0;
// }  

  // switch (case_status)
  // {
  // case 2:
  // setup_ap(sett,all_IMPLS,calcdata);
  //   break;

  // case 3:
  //   Serial.println(String("требуется натройка"));
  //   digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  //   delay(500);                       // wait for a second
  //   digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  //   delay(500);
  //   break;
  


  // default:
  //     calculate_values(sett,all_IMPLS,calcdata); 
  //     Serial.println(String(" cold:")+calcdata.data_COLD +String(" hot:")+calcdata.data_HOT + String(" elc1:")+calcdata.data_T1+ String(" elc2:")+calcdata.data_T2);
  //     //Serial.println(String(" caunt2: ")+all_IMPLS.caunt_elec_T2.impulses);
  //     //Serial.println(String(" cold:")+digitalRead(pinCold) +String(" hot:")+ digitalRead(pinHot) + String(" elc:")+ digitalRead(pinElectric));
  //     Serial.println(day()+String('.')+ month() +String('.')+ year() +String(' ')+hour()+String(':')+minute());
  //     Serial.println(String(sett.hostname_mqtt)+String("|"));
  //     delay(5000);  
  //   break;
  // }



}




