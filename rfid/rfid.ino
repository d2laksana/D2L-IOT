#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 2
#define SDA_PIN 0

MFRC522 mfrc522(SDA_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("Put your card to the reader...");

}

void loop() {
  if(!mfrc522.PICC_IsNewCardPresent()){
    return;
  }

  if(!mfrc522.PICC_ReadCardSerial()){
    return;
  }

  Serial.print("UID tag :");
  String content = "";
  byte letter;

  for(byte i = 0; i < mfrc522.uid.size; i++){
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  Serial.print("Pesan : ");
  content.toUpperCase();

  if(content.substring(1) == "35 D1 87 01"){
    Serial.println("Kartu cocok (KP)");
    Serial.println();
    delay(1000);
  }

  else if(content.substring(1) == "63 01 39 34"){
    Serial.println("Kartu cocok (GB)");
    Serial.println();
    delay(1000);
  }

  else{
    Serial.println("Kartu Tidak cocok");
    delay(1000);
  }

}