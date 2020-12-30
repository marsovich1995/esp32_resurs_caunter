#ifndef SETUP_H
#define SETUP_H
boolean check_celebrat();
struct SetUpData {  

    float    data_HOT;
    float    data_COLD;
    float    data_T1;
    float    data_T2;

    int liters_per_impuls;
    int impuls_per_kW;
    
/*
    const char *ssid;
    const char *password;

    uint32_t impulses3;   //           канал 3
    uint16_t adc0;        //Уровень,   канал 0
    uint16_t adc1;        //           канал 1
    uint16_t adc2;        //           канал 2
    uint16_t adc3;        //           канал 3*/
}; 


struct CauntIPLS {  

    uint32_t impulses;
    //static uint8_t pin;
    //uint8_t pause = 1000;
    volatile boolean Impuls = 0;
    volatile boolean lastImpuls =0;
    //volatile boolean ImpulsT1 = 0;  
  
    void pulse(uint8_t pin, int pause){
        static unsigned long last_interrupt_time = 0;
        unsigned long interrupt_time = millis();    
        Impuls =digitalRead(pin);
       
        if ( (interrupt_time-last_interrupt_time)>pause &&  (lastImpuls == 0 )) {
             impulses  ++;
             Serial.println("coll caunt");
        }
        last_interrupt_time = interrupt_time;
        lastImpuls = Impuls;
    }
};

struct DataAll_IMPLS
{
    CauntIPLS caunt_cold;
    CauntIPLS caunt_hot;
    CauntIPLS caunt_elec_T1;
    CauntIPLS caunt_elec_T2;
};


/*
        void pulse_E(uint8_t pin ){
        static unsigned long last_interrupt_time = 0;
        unsigned long interrupt_time = millis();    
        Impuls =digitalRead(pin);
       
        if ( (interrupt_time-last_interrupt_time)>1000 &&  (lastImpuls == 0 )) {
             if(check_celebrat() || weekday() == 1 || weekday() == 7 || hour()==23 || ( hour() <= 6 && minute() <= 59) ){
                impulses  ++;
             Serial.println("тариф 2");
            }else{
                ImpulsT1 ++;
                Serial.println("тариф 1");
            }
        }
        last_interrupt_time = interrupt_time;
        lastImpuls = Impuls;
       
    }*/ 

struct CalculatedData {
    float    data_HOT;
    float    data_COLD;
    float    data_T1;
    float    data_T2;
/*
    uint32_t delta0;
    uint32_t delta1;
    uint32_t delta2;
    uint32_t delta3;
 */   
    int8_t   rssi;
};


uint8_t celebration [][2] = {{1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},{1,8},{23,2},{8,3},{1,5},{9,5},{12,6},{4,11}};
boolean check_celebrat(){
  for (uint8_t i=0; i<14; i++){
      if (celebration[i][0]== day() && celebration[i][1] == month() /*&& year() > 2019 */){
        return true;
      }
  }
  return false;
}

#endif