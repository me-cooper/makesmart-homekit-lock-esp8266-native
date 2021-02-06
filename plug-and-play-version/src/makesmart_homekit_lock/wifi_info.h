#ifndef WIFI_INFO_H_
#define WIFI_INFO_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include <WiFiManager.h>


const char *ssid = "makesmartLock";
const char *password = "makesmart";

int builtinLED = 2;   //D4  PULL-UP LOW=ON

void blinkLed(int hold){
  pinMode(builtinLED, OUTPUT);

  for (int i = 0; i <= 26; i++) {

    if(digitalRead(builtinLED) == HIGH){
      digitalWrite(builtinLED, LOW);  
    }else{
      digitalWrite(builtinLED, HIGH);
    }
    
    delay(hold);
    
  }
  
}

void wifi_connect() {
    
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);

  WiFiManager wm;
  bool res;
  res = wm.autoConnect(ssid,password);

  if(!res) {
    ESP.restart();
  }else {
    Serial.printf("WiFi connected, IP: %s\n", WiFi.localIP().toString().c_str());
    blinkLed(400);
  }
  
}

#endif
