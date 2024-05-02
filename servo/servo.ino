// Include the library
#include "Servo.h"

// Create the servo object
Servo myservo;

// Setup section to run once
void setup() {
  Serial.begin(115200);
  myservo.attach(9); // attach the servo to our servo object
  myservo.write(90); // stop the motor
}

// Loop to keep the motor turning!
void loop() {
  myservo.write(45); // rotate the motor counter-clockwise
  Serial.println("rotate the motor counter-clockwise");
  delay(5000); // keep rotating for 5 seconds (5000 milliseconds)

  myservo.write(90); // stop the motor
  delay(5000); // stay stopped

  myservo.write(135); // rotate the motor clockwise
  Serial.println("rotate the motor clockwise");
  delay(5000); // keep rotating 😀
}