#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.println(testasm(0, 4));
  Serial.println(testasm(1, 4));
  Serial.println(testasm(2, 4));
  Serial.println(testasm(3, 4));
  Serial.println(testasm(4, 4));
  Serial.println(testasm(5, 4));
  Serial.println(testasm(6, 4));
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
