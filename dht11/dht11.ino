#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
// #include <ArduinoJson.h>

#define DHTPIN 2

#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

// String api = "http://api.d2l.my.id/suhu";
// String local = "http://192.168.137.2/suhu";
float hum;
float temp;

const char* ssid = "IoT";// 
const char* password = "iot_danudwiki";
// const size_t CAPACITY = JSON_OBJECT_SIZE(2);
// StaticJsonDocument<CAPACITY> doc;
// String data;
// HTTPClient http;
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
    // http.begin(client, api);
    // http.addHeader("Content-Type", "application/json");

    hum = dht.readHumidity();
    temp = dht.readTemperature(); 
    // postData();

    data = "";
    delay(60000);
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
