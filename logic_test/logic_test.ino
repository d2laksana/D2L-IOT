#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
String api = "http://api.d2l.my.id/suhu";
int value;
float hum;
float temp;

const char* ssid = "D2L";// 
const char* password = "@D2laksana";
const size_t CAPACITY = JSON_OBJECT_SIZE(2);
StaticJsonDocument<CAPACITY> doc;
String data;
HTTPClient http;
WiFiClient client;


void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  
}

void loop() {
  value = millis();
  if(WiFi.status()== WL_CONNECTED){ 
    if(value%60000==0){
      http.begin(client, api);
      http.addHeader("Content-Type", "application/json");

      hum = dht.readHumidity();
      temp = dht.readTemperature(); 
      postData();

      data = "";
    // delay(60000);
    } 
  }
}


void postData(){
  doc["Temp"] = temp;
  doc["Hum"] = hum;

  serializeJson(doc, data);
  
  int httpCode = http.POST(data);
  Serial.println();
  Serial.print("HTTP Response code is: ");
  Serial.println(httpCode);

  Serial.println(hum);
  Serial.println(temp);
  http.end();
}