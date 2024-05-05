// import library
#include "DHT.h"
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include   <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HX711.h"


// define dht
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
String resp;

// define oled
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT   64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected   to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,   &Wire, -1);

// hx711
const int LOADCELL_DOUT_PIN = 14;
const int LOADCELL_SCK_PIN = 12;
int berat;
int beratOld;

HX711 scale;

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

// time
unsigned long previousSuhuMillis = 0;  // Waktu sebelumnya untuk pembacaan suhu
unsigned long previousBeratMillis = 0; // Waktu sebelumnya untuk pembacaan berat

const long suhuInterval = 30000;  // Interval pembacaan suhu (dalam milidetik)
const long beratInterval = 60000;  // Interval pembacaan berat (dalam milidetik)

// suhu
float hum;
float temp;

// relay
const int relay = 16;

void setup() {
  Serial.begin(115200);

  // dht 
  dht.begin();

  // hx711
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(-487.0331); // hasil kalibrasi
  scale.tare(); 

  // oled
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))   { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
     for(;;);
  }
  display.display();
  delay(1000);
  display.clearDisplay();

  // wifi manager 
  res = wm.autoConnect("birdfarm", "birdfarm");
  // relay
  pinMode(relay, OUTPUT);

  Serial.println(".");
  Serial.println("Terhubung ke jaringan");

}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
      unsigned long currentMillis = millis();
      
      // read temp
      hum = dht.readHumidity();
      temp = dht.readTemperature();

      // post data
      if (currentMillis - previousSuhuMillis >= suhuInterval) {
        previousSuhuMillis = currentMillis; 
        postSuhu(temp, hum);
      }

      if (currentMillis - previousBeratMillis >= beratInterval) {
        previousBeratMillis = currentMillis; 
        Serial.println(berat);
        if (berat != beratOld) {
          postBerat(berat);
        }
      }

      // read data loadcell
      if(scale.wait_ready_timeout(5000)) {
        berat = round(scale.get_units());
        // Serial.println(berat);
      }

      // display oled
      display.clearDisplay();
      oledDisplayHeader();

      oledDisplay(3,3,20,berat,"g");
      oledDisplay(2,70,16,temp,"C");
      oledDisplay(2,80,45, hum, "%");

      display.display(); 
      
  }

}


void postSuhu(float temp, float hum){

  String uri = url + "iot/dht";
  http.begin(client, url+"iot/dht");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-api-key", apiKey);
  doc.clear();
  doc["temp"] = temp;
  doc["hum"] = hum;

  serializeJson(doc, resp);

  int httpCode = http.POST(resp);
  Serial.println();
  Serial.print("HTTP Response code is: ");
  Serial.println(httpCode);
  Serial.println(http.getString().c_str());

  Serial.println(resp);
  resp = "";
  http.end();
}

void oledDisplayHeader(){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Berat");
  display.setCursor(60,   0);
  display.print("Temperature");
  display.setCursor(75, 35);
  display.print("Humidity");
}
void oledDisplay(int size, int x,int   y, float value, String unit){
  int charLen=12;
  int xo=x+charLen*3.2;
  int xunit=x+charLen*3.6;
  int xval = x; 
  display.setTextSize(size);
  display.setTextColor(WHITE);
   
 if (unit=="%"){
    display.setCursor(x, y);
    display.print(value,0);
    display.print(unit);
 } else if (unit=="g"){
    display.setCursor(x, y);
    display.print(value,0);
    display.print(unit);
 } else {
    if (value>99){
      xval=x;
    }   else {
      xval=x+charLen;
    }
    display.setCursor(xval, y);
    display.print(value,0);
    display.drawCircle(xo, y+2, 2, WHITE);  // print degree symbols (  )
    display.setCursor(xunit,   y);
    display.print(unit);
 }
}

void postBerat(int w) {
  String uri = url + "iot/volume";
  http.begin(client, url+"iot/volume");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-api-key", apiKey);
  
  doc.clear();
  doc["value"] = w;

  serializeJson(doc, resp);

  int httpCode = http.POST(resp);
  Serial.println();
  Serial.print("HTTP Response code is: ");
  Serial.println(httpCode);
  Serial.println(http.getString().c_str());

  Serial.println(resp);
  resp = "";
  http.end();
}
