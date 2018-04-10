#include "concurrency.h"
int led_pin = 13;
int white = 3;
int blue = 2;

void p1 (void)
{
  /* process 1 here */
  while(1) {
    digitalWrite (white, HIGH);
    delay (500);
    digitalWrite (white, LOW);
    delay (500);
  }
}

void p2 (void)
{
/* process 2 here */
    while(1)  {
      digitalWrite (blue, HIGH);
      delay (500);
      digitalWrite (blue, LOW);
      delay (500);
    } 
    
}

void setup()
{
   pinMode(led_pin, OUTPUT);
   pinMode(white, OUTPUT);
   pinMode(blue, OUTPUT);
   Serial.begin(9600);
   
   if (process_create (p1, 32) < 0) {
      Serial.println("p1 dead");
      return;
   }
 
   if (process_create (p2, 32) < 0) {
      Serial.println("p2 dead");
      return;
   }
   
}

void loop()
{
 // p1();
  process_start();
  
}

