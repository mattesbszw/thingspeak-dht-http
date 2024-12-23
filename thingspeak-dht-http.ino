// #define PRINT_DEBUG_MESSAGES // not necessary but useful for debugging TS library

#include "tscert.h"

#include <DHT.h>
#include <WiFi.h>
#include <ThingSpeak.h> // unbedingt nach der WiFi-Bibliothek einbinden


#define DHT_PIN 4
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);

// WLAN-Zugangsdaten
const char* ssid = "CPSLABOR";
const char* password = "A1234567890";

WiFiClient client;

// auf den jew. ThingSpeak-Kanal anpassen
unsigned long tsChannelNumber = 0;
const char* tsWriteAPIKey = "YOURKEYHERE";


void setup(void){
  
  Serial.begin(115200);
  dht.begin(); // init DHT sensor
  
  connectWiFi();
     
  // ThingSpeak API wird initialisiert
  ThingSpeak.begin(client);
  
}

void connectWiFi() {
  
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to " + String(ssid));
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    WiFi.begin(ssid, password); 
    delay(5000);     
  } 
  Serial.println("\nConnected.");  
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}


void loop(void){ 
  
  float hTmp = dht.readTemperature();
  String dhtTemperature = "0";
  if (!isnan(hTmp)) {
    dhtTemperature = hTmp;
  }
  Serial.print("Sensor (*C): "); 
  Serial.println(dhtTemperature);
 
  float hHum = dht.readHumidity();
  String dhtHumidity = "0";
  if (!isnan(hHum)) {
    dhtHumidity = hHum;
  }
  Serial.print("Sensor (%): "); 
  Serial.println(dhtHumidity);

  ThingSpeak.setField(1, dhtTemperature);
  ThingSpeak.setField(2, dhtHumidity);
  int ret = ThingSpeak.writeFields(tsChannelNumber, tsWriteAPIKey);
   
   if(ret == 200){
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(ret));
  } 
  
  delay(30000);
}
