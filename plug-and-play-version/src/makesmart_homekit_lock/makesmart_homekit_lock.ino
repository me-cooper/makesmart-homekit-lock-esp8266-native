#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

int relay = 15;   // D8   PULL-DOWN 
void setup() {
  pinMode(relay, OUTPUT);
  Serial.begin(115200);
  wifi_connect();
  //homekit_storage_reset();
  my_homekit_setup();
}

void loop() {
  my_homekit_loop();
  delay(10);
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
  cha_lock_current_state.value.int_value = state;
  if(state == 0){
    // Schloss wurde via HomeKit geöffnet
    open_lock();
  }
  if(state == 1){
    // Schloss wurde via HomeKit geschlossen
    close_lock();
  }
  // HomeKit-Status aktualisieren
  homekit_characteristic_notify(&cha_lock_current_state, cha_lock_current_state.value);
}

void my_homekit_setup() {
  
  cha_lock_target_state.setter = set_lock;
  arduino_homekit_setup(&config);

  
}


void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_heap_millis) {
    // heap-Info alle 30 Sekunden im seriellen Monitor ausgeben
    next_heap_millis = t + 30 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
        ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }
}



/* Diese beiden Funktionen werden aufgerufen, wenn das Schloss in HomeKit betätigt wird */
void open_lock(){
  Serial.println("Schloss öffnen"); 
  // hier beliebige Aktion einfügen
  digitalWrite(relay, HIGH);
}

void close_lock(){
  Serial.println("Schloss schließen");  
  // hier beliebige Aktion einfügen
  digitalWrite(relay, LOW);
}
