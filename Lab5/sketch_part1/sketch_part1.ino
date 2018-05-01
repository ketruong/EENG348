#include "concurrency.h"

lock_t m;
int flag = 1 ;
void blink1 (void)
{
  while (1) {
    lock_acquire (&m);
    digitalWrite (8, HIGH);
    delay (700);
    digitalWrite (8, LOW);
    delay (700);
    lock_release (&m);
  }
}

void blink2 (void)
{
   while (1) {
      digitalWrite (10, HIGH);
      delay (500);
      digitalWrite (10, LOW);
      delay (500);
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
}

void blink7 (void)
{
    digitalWrite (8, HIGH);
    delay (500);
    digitalWrite (8, LOW);
    delay (500);
    digitalWrite (8, HIGH);
    delay (500);
    digitalWrite (8, LOW);
    delay (500);    
}

void blink8 (void)
{
    digitalWrite (10, HIGH);
    delay (500);
    digitalWrite (10, LOW);
    delay (500);
    digitalWrite (10, HIGH);
    delay (500);
    digitalWrite (10, LOW);
    delay (500);   
     digitalWrite (10, HIGH);
    delay (500);
    digitalWrite (10, LOW);
    delay (500);     
}

void blink9 (void)
{
    while(1) {
      digitalWrite (13, HIGH);
      delay (500);
      digitalWrite (13, LOW);
      delay (500);
      digitalWrite (13, HIGH);
      delay (500);
      digitalWrite (13, LOW);
      delay (500);   
    } 
}

void blink10 (void)
{
      lock_acquire (&m);
      digitalWrite (8, HIGH);
      delay (500);
      digitalWrite (8, LOW);
      delay (500);
      digitalWrite (8, HIGH);
      delay (500);
      digitalWrite (8, LOW);
      delay (500);
      lock_release (&m);   
}

// one locked process
// Expected behavior: LEDs 8 and 10 blink at their assigned
// intervals. Process 1 is not in competition for the lock, so
// LED 8's blinking pattern is as expected. LED 9 is starved.
// This behavior is observed.
void lock_1 (){
  process_create_prio(blink1, 32, 200);
  process_create_prio(blink2, 32, 200);
  process_create_prio(blink3, 32, 100);
}

// two locked processes
// Expected behavior: same as in lock_5, although processes
// 1 and 4 compete for the lock on the same LED, leading to
// a pattern of rapid, then slow blinking in LED 8.
// This behavior is observed.
void lock_2 (){
  process_create_prio(blink1, 32,128);
  process_create(blink2, 32);
  process_create(blink3, 32);
  process_create_prio(blink4, 32,128);
}

// three locked proesses.
// Expected behavior: Only LED 10 blinks since it 
// is the only real time job and it doesn't terminate
// 
// This behavior is observed 
void lock_3(){
  process_create_rtjob(blink1, 32,200,200);
  process_create(blink3, 32);
  process_create_prio(blink4, 32,140);
  process_create_prio(blink5, 32,128);
}

// only locked processes
// Expected behavior: LED 8 competes for the lock,
// resulting in only LED 8 running since all other 
// since all other processes are starved.
// This behavior is observed.
void lock_4(){
  process_create_prio(blink1, 32,128);
  process_create_prio(blink4, 32,130);
  process_create_prio(blink5, 32,128);
}

// only unlocked processes
// Expected behavior: LEDs 13 blinks in tandem without
// termination since it has the highest priority
// This behavior is observed.
void lock_5(){
  process_create_rtjob(blink2, 32,200,200);
  process_create_rtjob(blink3, 32,200,300);
  process_create_rtjob(blink9, 32,200,100);
  //process_create(blink10, 32);
}

// no processes
// Expected behavior: no LEDs are active.
// This behavior is observed.
void lock_6(){
  
}

// one terminating process
// Expected behavior: LEDs 9 and 10 are not in competition
// for the lock, and accordingly blink freely. LED 9, however,
// only blinks twice before terminating. Meanwhile, LEDs 8 and
// 13 compete for the lock, resulting in alternation between
// the two blinking patterns.
// This behavior is observed.
void lock_7(){
  process_create (blink1, 32);
  process_create (blink2, 32);
  process_create (blink4, 32);
  process_create (blink5, 32);
  process_create (blink6, 32);
}

// three terminating processes
// Expected behavior: LED 8 blinks twice, 10 blinks twice and 9 
// blinks once, an they all terminate.
// This behavior is observed.
void lock_8(){
  process_create_rtjob(blink6, 32,200,300);
  process_create_rtjob(blink7, 32,200,100);
  process_create_rtjob(blink8, 32,200,200);
}

// Only LED 13  acquired the lock and blinks.
// The rest starve until blink10 terminates
// This behaviour is observed.
void lock_9(){
  process_create_rtjob (blink10, 32,129, 129);
  process_create (blink4, 32);
  process_create (blink5, 32);
}

void setup() {
  // put your setup code here, to run once: 
  pinMode (8, OUTPUT);
  pinMode (9, OUTPUT);
  pinMode (10, OUTPUT);
  pinMode (13, OUTPUT);
  Serial.begin (9600);
  lock_init (&m);
  
  Serial.println("Lock Created Successfully!");
  lock_7();
}

void loop() {
  Serial.println ("start...");
  process_start();
  Serial.println ("done!");
}
