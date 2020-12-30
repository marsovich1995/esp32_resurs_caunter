#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include <EEPROM.h>


#define lOGIN_LEN 16
#define HOST_LEN 32
#define DESCRIP_LEN 64
#define ESP_CONNECT_TIMEOUT 15000



struct SetUpData {  
// Начальные значения счетчиков
    float    data_HOT;
    float    data_COLD;
    float    data_T1;
    float    data_T2;

    int liters_per_impuls; // ипллсов на литр
    int impuls_per_kW; // количество иплусов на уиловатт
    char hostname_mqtt[HOST_LEN]; // адрес сервера mqtt
    uint16_t port_mqtt;
    char mqtt_user[lOGIN_LEN];
    char mqtt_password[lOGIN_LEN];
    
    char description[DESCRIP_LEN]; // описание устройства
    uint16_t period_send; // период отправки данных
    uint8_t TimeZone;

    boolean target_restart;
    uint8_t crc_cng; // ??контрольная сумма
    uint8_t first_start; //ключь первого запуска
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




struct CalculatedData {
    float    data_HOT;
    float    data_COLD;
    float    data_T1;
    float    data_T2;
   
    int8_t   rssi;
};


const uint8_t celebration [][2] = {{1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},{1,8},{23,2},{8,3},{1,5},{9,5},{12,6},{4,11}};

boolean check_celebrat();
void storeConfig( SetUpData &sett);
boolean loadConfig( SetUpData &sett);



#endif