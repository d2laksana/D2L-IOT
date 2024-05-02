// import library
#include "DHT.h"
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


// define dht
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
String data;

// define wifi manager
WiFiManager wm;
bool res;

// http client
HTTPClient http;
WiFiClient client;

// define api
String url = "http://birdfarm.d2l.my.id/api/";
String apiKey = "76b7ab89a4f28e3e667654ea4048e3843945d6be45a6a53c01e3ee841627d7fc";
JsonDocument doc;

void setup() {
  Serial.begin(115200);

  // dht 
  dht.begin();
  
  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(60);
  res = wm.autoConnect("birdfarm", "birdfarm");
  while (!res) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println(".");
  Serial.println("Terhubung ke jaringan");

}

void loop() {
  wm.process();
  if (res) {
    String uri = url + "iot/dht";
    Serial.println(uri);
    http.begin(client, url+"iot/dht");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("x-api-key", apiKey);
    float hum = dht.readHumidity();
    float temp = dht.readTemperature(); 
    postData(temp, hum);


    delay(10000);
  }

}


void postData(float temp, float hum){
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
