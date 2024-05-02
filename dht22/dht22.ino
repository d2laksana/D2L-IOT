#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define DHTPIN 2

#define DHTTYPE DHT22

DHT dht(DHTPIN,DHTTYPE);

// String api = "http://api.d2l.my.id/suhu";
// String url = "http://192.168.137.1:3001/api/suhu/store";
String url = "http://birdfarm.d2l.my.id/api/suhu/store";
String apiKey = "8352a2839987a5502e62b73f9d3c728b8072f342d62466881bcad3b8bb1b9536";
float hum;
float temp;

const char* ssid = "DAN";// 
const char* password = "@D2laksana";
const size_t CAPACITY = JSON_OBJECT_SIZE(2);
StaticJsonDocument<CAPACITY> doc;
String data;
HTTPClient http;
WiFiClient client;

 
void setup(){
  Serial.begin(9600);
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


void loop(){
  if(WiFi.status()== WL_CONNECTED){ 
    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("x-api-key", apiKey);
    hum = dht.readHumidity();
    temp = dht.readTemperature(); 
    postData();
    data = "";
    delay(10000);
  }
}


void postData(){
  doc["temp"] = temp;
  doc["hum"] = hum;

  serializeJson(doc, data);
  
  int httpCode = http.POST(data);
  Serial.println();
  Serial.print("HTTP Response code is: ");
  Serial.println(httpCode);
  Serial.println(http.getString().c_str());

  Serial.println(data);
  http.end();
}
