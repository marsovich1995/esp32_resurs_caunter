
// #include <led.h>




// void blink_Error(void *pvParam){
// pinMode(LED, OUTPUT);
// ledmode_t ledmode = LED_OFF;
// digitalWrite(LED, 0);
// while (true)
//   {
//     uint32_t notifyValue;
//     if (xTaskNotifyWait(0,0,&notifyValue, ledmode < LED_1HZ ? portMAX_DELAY : 0)== pdTRUE){
//       ledmode = (ledmode_t)notifyValue;
//       if (ledmode == LED_OFF)
//         digitalWrite (LED, 0);
//       else if (ledmode == LED_ON)
//         digitalWrite (LED, 1);
//     }
//     if (ledmode >= LED_1HZ){
//       digitalWrite (LED, !digitalRead(LED));
//       vTaskDelay(pdMS_TO_TICKS((ledmode == LED_1HZ ? 1000 : ledmode == LED_2HZ ? 500 : ledmode == LED_4HZ ? 250 : 125)));
//     }
//   }
// }

// void set_blink(ledmode_t ledmode){
//     if (eTaskGetState(blink) == eSuspended) vTaskResume(blink);
//     if (xTaskNotify(blink,ledmode, eSetValueWithoutOverwrite) != pdPASS){
//         Serial.println("Error seting LED mode");
//     }
// }

// void Create_LED_Task(){
// xTaskCreate(blink_Error,"Blink", 1024, NULL, 1 , &blink);
// }

// void Stop_LED_Task(){
// vTaskSuspend(blink);
// }
// void Start_LED_Task(){
// vTaskResume(blink);
// }