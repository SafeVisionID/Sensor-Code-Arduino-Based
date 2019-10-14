//Import Library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
#include <EEPROM.h>

String message = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  EEPROM.begin(512);
  message = Serial.read();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<=message.length();i++){
    EEPROM.write(i,message[i]);
//    Serial.println(message[i]);  
  }
  EEPROM.commit();
  for(int i=0;i<=message.length();i++){
    Serial.println(EEPROM.read(i));
  }
  delay(1000);
}
