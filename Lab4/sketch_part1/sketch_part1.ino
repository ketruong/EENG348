#include "concurrency.h"
int led_pin = 13;
int led_pin1 = 10;
int blue = 9;
int green = 8;

void p1 (void)
{
  /* process 1 here */
  while(1) {
    digitalWrite (green, HIGH);
    delay (500);
    digitalWrite (green, LOW);
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

void p3 (void)
{  
      digitalWrite (led_pin, HIGH);
      delay (500);
      digitalWrite (led_pin, LOW);
      delay (500);
       digitalWrite (led_pin, HIGH);
      delay (500);
      digitalWrite (led_pin, LOW);
      delay (500);  
}

void p4 (void)
{  
      digitalWrite (led_pin1, HIGH);
      delay (500);
      digitalWrite (led_pin1, LOW);
      delay (500);
       digitalWrite (led_pin1, HIGH);
      delay (500);
      digitalWrite (led_pin1, LOW);
      delay (500);  
}

void setup()
{
 
   pinMode(green, OUTPUT);
   pinMode(blue, OUTPUT);
   pinMode(led_pin, OUTPUT);
   Serial.begin(9600);
   
   if (process_create (p1, 32) < 0) {
      return;
   }
 
   if (process_create (p2, 32) < 0) {
      return;
   }
   // terminating process 
   if (process_create (p3, 32) < 0) {
      return;
   }

   // terminating process
   if (process_create (p4, 32) < 0) {
      return;
   }
   
}

void loop()
{
  process_start(); 
}

