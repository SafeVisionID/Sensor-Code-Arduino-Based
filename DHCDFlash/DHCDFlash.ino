#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
#include <EEPROM.h>
#define EEPROM_SIZE 1

//SSID and Password to your ESP Access Point
const char* ssid = "DoorSensor12345";
const char* password = "ReadyToGo";
ESP8266WebServer server(80); //Server on port 80
#define USE_SERIAL Serial
int lampu = D1;
String ssid_on = "";
String pass_on = "";
String username = "";

void handleText() {
  String message = "";
  for (uint8_t i=0; i<server.args(); i++){
    message += server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(200, "text/plain", message);
  Serial.println(message);
  ssid_on = 
  EEPROM.write(0,);
  EEPROM.commit();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("");
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
 
  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpt IP:");
  Serial.println(myIP);
 
  server.on("/text", handleText); //Which routine to handle at root location
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
