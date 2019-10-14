//Import Library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <SocketIoClient.h>

//Define Some Variables
#define USE_SERIAL Serial
#define pir     D8
#define magnet  D6
#define resetButton D7
#define setStatusMode D5

//Define values
int Smagnet, Spir = 0;
int Dpir, Dmagnet;
const char* ssid = "DoorSensor12345";
const char* password = "ReadyToGo";
int lampu = D1;
String ssid_station = "";
String pass_station = "";
int resetStatus = 0;
String username = "";
const size_t capacity = JSON_OBJECT_SIZE(3);

ESP8266WebServer server(80); 
DynamicJsonDocument doc(capacity);
ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;

void handleText() {
  String message = "";
  for (uint8_t i=0; i<server.args(); i++){
    message += server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(200, "text/plain", message);
  Serial.println(message);
  
  for(int i=0;i<=message.length();i++){
    EEPROM.write(i,message[i]);
    EEPROM.commit();
  }
  
}

void checkSSIDPass(){
  
}

void readsensor() {
  long Spir = digitalRead(pir);  // Sensor pir
  Serial.print("Spir: ");
  Serial.println(Spir);
  if (Spir == 1) {
    Dpir = 1;
    Serial.print("Read digital input: ");
    Serial.println(Dpir);
  } else {
    Dpir = 0;
    Serial.print("Read digital input: ");
    Serial.println(Dpir);
  }

  boolean btnState = digitalRead(magnet); // Sensor Magnet
  if (btnState != LOW) {
    Serial.println("The Door Is OPEN");
    Dmagnet = 1;
  } else {
    Dmagnet = 0;
  }
}

void event(const char * payload, size_t length) {
  USE_SERIAL.printf("got message: %s\n", payload);
}

void modeStation(){
   USE_SERIAL.begin(9600);

    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

     pinMode(D2, OUTPUT);
     digitalWrite(D2, !digitalRead(D2));

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    WiFiMulti.addAP("MEDOKAN BARU IV NO 12", "widyastuti");

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    webSocket.on("sensor_1_push", event);
    webSocket.begin("167.71.214.196",6500);
//    webSocket.begin("192.168.0.13",80,"/websocket");
    // use HTTP Basic Authorization this is optional remove if not needed
//    webSocket.setAuthorization("username", "password");
}

void modeSetting(){
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

void runStation(){
   webSocket.loop();
    
    readsensor();
    
    doc["id"] = "A54565";
    doc["pintu"] = Dmagnet;
    doc["motion"] = Dpir;
    char output[128];
    serializeJson(doc, output);
    
    Serial.println("Motion: "+String(Dpir));
    Serial.println( "Door: "+ String(Dmagnet));
    webSocket.emit("sensor_1_push", output);
    
    delay(1000);
}

void runSetting(){
    // put your main code here, to run repeatedly:
    server.handleClient();
}



void setup() {
  pinMode(pir, INPUT_PULLUP);      
  pinMode(magnet, INPUT_PULLUP);       
  pinMode(resetButton, OUTPUT);
  pinMode(setStatusMode,INPUT);

  int statusMode = digitalRead(setStatusMode);
  resetStatus = statusMode;
  if(statusMode == 1){
    Serial.println("Mode Setting!");   
    modeSetting();
//    EEPROM.write(72, 'M');
//    EEPROM.commit();
  }else if(statusMode == 0){
    Serial.println("Mode Station!");
//    String SSID = EEPROM.read(0);   
    modeStation();
  }else{
    Serial.println("Mode Setting!");   
    modeSetting();
  }

}

void setWiFi(String newSSID, String newPass){
  
}

void getWifi(){
  
}

void loop() {

  digitalWrite(resetButton,LOW);
  int statusMode = digitalRead(setStatusMode);
  if(resetStatus != statusMode){
    digitalWrite(resetButton,HIGH);
    delay(500);
    digitalWrite(resetButton,LOW);
  }
  
  Serial.println("statusMode: "+String(statusMode));
  
  if(statusMode == 1){
    Serial.println("Mode Setting!");   
    runSetting();
    resetStatus = 1;
  }else if(statusMode == 0){
    Serial.println("Mode Station!");   
    runStation();
    resetStatus = 0;
  }else{
    Serial.println("Mode Setting!");   
    runSetting();
  }

  delay(1000);
}
