#include "concurrency.h"

/**
   Simple blinking LED example using concurrency.

   Here I've assumed you have LEDs hooked up to the following pins:
    
   13 (this is the built-in LED typically)
    2
    3

**/

lock_t m;

void blink1 (void)
{
  while (1) {
    lock_acquire (&m);
    digitalWrite (4, HIGH);
    delay (500);
    digitalWrite (4, LOW);
    delay (500);
    lock_release (&m);
  }
}

void blink2 (void)
{
   while (1) {
      digitalWrite (2, HIGH);
      delay (500);
      digitalWrite (2, LOW);
      delay (500);
   }
}


void blink3 (void)
{
   while (1) {
      digitalWrite (3, HIGH);
      delay (500);
      digitalWrite (3, LOW);
      delay (500);
   }
}

void blink4 (void)
{
  while (1) {
      lock_acquire (&m);
      digitalWrite (4, HIGH);
      delay (100);
      digitalWrite (4, LOW);
      delay (100);
      digitalWrite (4, HIGH);
      delay (100);
      digitalWrite (4, LOW);
      delay (100);
      lock_release (&m);
  }
}

void setup() {
  // put your setup code here, to run once: 
  pinMode (2, OUTPUT);
  pinMode (3, OUTPUT);
  pinMode (4, OUTPUT);
  pinMode (5, OUTPUT);
  Serial.begin (9600);
  lock_init (&m);
  
  Serial.println("Lock Created Good");

  //Serial.println(m.locked);
  process_create (blink1, 32);
  process_create (blink2, 32);

  Serial.println("Blink 2 Good");
  process_create (blink3, 32);
  Serial.println("Blink 3 Good");
  process_create (blink4, 32);
  Serial.println("Blink 4 Good");
}


void loop() {
  Serial.println ("start...");
  process_start ();
  Serial.println ("done!");
}
