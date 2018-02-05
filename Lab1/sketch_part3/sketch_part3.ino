#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.println(testasm(0));
  Serial.println(testasm(1));
  Serial.println(testasm(2));
  Serial.println(testasm(3));
  Serial.println(testasm(4));
  Serial.println(testasm(5));
  Serial.println(testasm(6));
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
