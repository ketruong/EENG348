#include "concurrency.h"

lock_t m;

void blink1 (void)
{
  while (1) {
    lock_acquire (&m);
    digitalWrite (8, HIGH);
    delay (500);
    digitalWrite (8, LOW);
    delay (500);
    lock_release (&m);
  }
}

void blink2 (void)
{
   while (1) {
      //lock_acquire (&m);
      digitalWrite (10, HIGH);
      delay (500);
      digitalWrite (10, LOW);
      delay (500);
      //lock_release (&m);
   }
}


void blink3 (void)
{
   while (1) {
      digitalWrite (9, HIGH);
      delay (500);
      digitalWrite (9, LOW);
      delay (500);
  }
}

void blink4 (void)
{
  while (1) {
      lock_acquire (&m);
      digitalWrite (8, HIGH);
      delay (100);
      digitalWrite (8, LOW);
      delay (100);
      digitalWrite (8, HIGH);
      delay (100);
      digitalWrite (8, LOW);
      delay (100);
      lock_release (&m);
  }
}

void blink5 (void)
{
  while (1) {
    lock_acquire (&m);
    digitalWrite (13, HIGH);
    delay (200);
    digitalWrite (13, LOW);
    delay (200);
    lock_release (&m);
  }
}

void blink6 (void)
{
    digitalWrite (9, HIGH);
    delay (500);
    digitalWrite (9, LOW);
    delay (500);
    digitalWrite (9, HIGH);
    delay (500);
    digitalWrite (9, LOW);
    delay (500);    
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

// one terminate process
void lock_7(){
  process_create (blink1, 32);
  process_create (blink2, 32);
  process_create (blink4, 32);
  process_create (blink5, 32);
  process_create (blink6, 32);
}


void setup() {
  // put your setup code here, to run once: 
  pinMode (8, OUTPUT);
  pinMode (9, OUTPUT);
  pinMode (10, OUTPUT);
  pinMode (13, OUTPUT);
  Serial.begin (9600);
  lock_init (&m);
  
  Serial.println("Lock Created Good");

  lock_7();
}

void loop() {
  Serial.println ("start...");
  process_start ();
  Serial.println ("done!");
}
