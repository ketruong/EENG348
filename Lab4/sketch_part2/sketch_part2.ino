#include "concurrency.h"

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
      //lock_acquire (&m);
      digitalWrite (2, HIGH);
      delay (500);
      digitalWrite (2, LOW);
      delay (500);
      //lock_release (&m);
   }
}


void blink3 (void)
{
   while (1) {
      //lock_acquire (&m);
      digitalWrite (3, HIGH);
      delay (500);
      digitalWrite (3, LOW);
      delay (500);
      //lock_release (&m);
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

void blink5 (void)
{
  while (1) {
    lock_acquire (&m);
    digitalWrite (5, HIGH);
    delay (200);
    digitalWrite (5, LOW);
    delay (200);
    lock_release (&m);
  }
}

// one locked process
void lock_1 (){
  process_create (blink1, 32);
  process_create (blink2, 32);
  process_create (blink3, 32);
}

// two locked processes
void lock_2 (){
  process_create (blink1, 32);
  process_create (blink2, 32);
  process_create (blink3, 32);
  process_create (blink4, 32);
}

//three locked process
void lock_3(){
  process_create (blink1, 32);
  process_create (blink2, 32);
  process_create (blink3, 32);
  process_create (blink4, 32);
  process_create (blink5, 32);
}

// only locked processes
void lock_4(){
  process_create (blink1, 32);
  process_create (blink4, 32);
  process_create (blink5, 32);
}

// only unlocked processes
void lock_5(){
  process_create (blink2, 32);
  process_create (blink3, 32);
}

// no processes
void lock_6(){
  
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

  lock_3();
}


void loop() {
  Serial.println ("start...");
  process_start ();
  Serial.println ("done!");
}
