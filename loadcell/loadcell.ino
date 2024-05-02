#include "HX711.h"


// circuit wiring
const int LOADCELL_DOUT = 12;
const int LOADCELL_SCK = 13;

HX711 scale;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  scale.begin(LOADCELL_DOUT, LOADCELL_SCK);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (scale.is_ready()) {
    scale.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
  } 
  else {
    Serial.println("HX711 not found.");
  }
  delay(1000);

}
