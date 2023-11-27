#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

#define EVERY_N_MILLISECONDS(interval) for (static unsigned long _lastTime = millis(); millis() - _lastTime >= (interval); _lastTime = millis())


void setup() {
  Serial.begin(115200);
  
  wifi_connect();
  
  homekit_storage_reset();
  
  my_homekit_setup();
  
}

void loop() {

  EVERY_N_MILLISECONDS(10) {
    my_homekit_loop();
  }
  

  
}

//==============================
// HomeKit setup and loop
//==============================

// Zugriff auf die Definitionen des Accessories in my_accessory.c
extern "C" homekit_server_config_t config;

extern "C" homekit_characteristic_t cha_lock_current_state;
extern "C" homekit_characteristic_t cha_lock_target_state;

static uint32_t next_heap_millis = 0;


// Diese Funktion wird aufgerufen, wenn das Schloss via HomeKit betätigt wird
void set_lock(const homekit_value_t value) {
 
  uint8_t state = value.int_value;
 
  if(state == 0){
    open_lock();
  }

 
  if(state == 1){
    close_lock();
  }
  
  
}

void my_homekit_setup() {
  
  cha_lock_target_state.setter = set_lock;
  arduino_homekit_setup(&config);

  
}


void my_homekit_loop() {
  
  arduino_homekit_loop();

  EVERY_N_MILLISECONDS(30000){
    LOG_D("Free heap: %d, HomeKit clients: %d", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());  
  }
  
  
}



/* Diese beiden Funktionen werden aufgerufen, wenn das Schloss in HomeKit betätigt wird */
void open_lock(){
  // Set Target State
  cha_lock_target_state.value.int_value = 0;
  homekit_characteristic_notify(&cha_lock_target_state, cha_lock_target_state.value); 



  /* ############################################################################################## */
  /*                     Hier beliebigen Code einfügen, der ein Schloss öffnet                      */
  /* ############################################################################################## */
  Serial.println("Schloss öffnen");

  

  // Set Current State
  cha_lock_current_state.value.int_value = 0;
  homekit_characteristic_notify(&cha_lock_current_state, cha_lock_current_state.value);
}

void close_lock(){
  // Set Target State
  cha_lock_target_state.value.int_value = 1;
  homekit_characteristic_notify(&cha_lock_target_state, cha_lock_target_state.value); 



  /* ############################################################################################## */
  /*                    Hier beliebigen Code einfügen, der ein Schloss schließt                     */
  /* ############################################################################################## */
  Serial.println("Schloss schließen");  



  // Set Current State
  cha_lock_current_state.value.int_value = 1;
  homekit_characteristic_notify(&cha_lock_current_state, cha_lock_current_state.value);  
}
