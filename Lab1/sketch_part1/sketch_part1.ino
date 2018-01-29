#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.print("The sum of 3 and 4 is: ");
  Serial.println(testasm(3, 4));
  Serial.print("The sum of 50 and 50 is: ");
  Serial.println(testasm(50, 50));
  Serial.print("The sum of 100 and 100 is: ");
  Serial.println(testasm(100, 100));
  Serial.print("The sum of 255 and 255 is: ");
  Serial.println(testasm(255, 255));
  Serial.print("The sum of 240 and 245 is: ");
  Serial.println(testasm(240, 245));
  Serial.print("The sum of 220 and 215 is: ");
  Serial.println(testasm(220, 215));
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
