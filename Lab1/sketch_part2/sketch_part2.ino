#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.print("(10 + 5) + |10 - 5| = ");
  Serial.print(testasm(10, 5));
  Serial.println(" (20 expected)");
  Serial.print("(10 + 20) + |10 - 20| = ");
  Serial.print(testasm(10, 20));
  Serial.println(" (40 expected)");
  Serial.print("(40 + 20) + |40 - 20| = ");
  Serial.print(testasm(40, 20));
  Serial.println(" (80 expected)");
  Serial.print("(100 + 100) + |100 - 100| = ");
  Serial.print(testasm(100, 100));
  Serial.println(" (200 expected)");
  Serial.print("(200 + 150) + |200 - 150| = ");
  Serial.print(testasm(200, 150));
  Serial.println(" (400 expected)");
  Serial.print("(250 + 0) + |250 - 0| = ");
  Serial.print(testasm(250, 250));
  Serial.println(" (500 expected)");
  Serial.print("(247 + 3) + |247 - 3| = ");
  Serial.print(testasm(247, 3));
  Serial.println(" (494 expected)");
  Serial.print("(245 + 9) + |245 - 9| = ");
  Serial.print(testasm(245, 9));
  Serial.println(" (490 expected)");
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
