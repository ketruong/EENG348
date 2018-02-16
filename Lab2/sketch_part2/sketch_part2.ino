int clockPin = 10;
int latchPin = 9;
int dataPin = 8;
int clockPinx = 13;
int latchPinx = 12;
int dataPinx = 11;

// Bottom to Top
byte ledStates[10][4] = { {B1000001,B1111111,B1000001,B1111111}, //0
                          {B0000100, B0000010, B1111111,B1000000},//1
                          {B1001001,B0000111,B1110000,B1110000},//2
                          {B1001001,B1111111,B1001001,B1111111},//3
                          {B0001000,B0001111,B0001000,B1111111},//4
                          {B1001001,B0000111,B1110000,B0000000},//5
                          {B1001001,B111001,B111111,B0000000},//6
                          {B0000001,1111111,B0000001,B1111111},//7
                          {B1001001,B0001111,B1001001,B1111111},//8
                          {B0001001,B0001111,B0001001,B1111111}//9
                          }; 
//Left to Right 
byte GroundLEDs [10][4] = { {B00000,B01110,B00000,B01110}, //0
                            {B01111, B10111, B11011,B00000},//1
                            {B00000,B11110,B01111,B01111},//2
                            {B00000,B11110,B00000,B11110},//3
                            {B00000,B01111,B00000,B11110},//4
                            {B00000,B01111,B11110,B01111},//5
                            {B00000,B01110,B01111,B01111},//6
                            {B00000,B11110,B00000,B11110}, //7
                            {B00000,B01110,B00000,B01110},//8
                            {B00000,B01111,B00000,B11110}//9
                            }; 


void setup() {
    // Set pins 2-7 as output
    DDRB = B11111111;
    // Set latch as low
    PORTB = B00000000;
    shiftOut(dataPin, clockPin, LSBFIRST, B11111111); 
    // Set latch as high
    PORTB = B00000010; 
}

void loop() {
  // Loops through 0-9 digits
  for(int k =0; k<=9;k++){ 
    // Displays for 1 sec
    for(int j =0; j<=1000; j++){
      // Go through parts that make the digit 
      for(int i=0;i<=3;i++){
         // Get rows 
         SetStates(ledStates[k][i]);
         // Get Columns
         GroundCorrectLED (GroundLEDs[k][i]);

         // Set latch as low
         PORTB = B00000000;
         shiftOut(dataPin, clockPin, LSBFIRST, B11111111); 
         // Set latch as high
        PORTB = B00000010;
      }
    }
    delay(100);
    }
}
// Controls Columns 
void GroundCorrectLED (byte states){
    // Set latch to low 
    PORTB = B00000000;
    shiftOut(dataPin, clockPin, LSBFIRST, states); 
    // Set latch to high
    PORTB = B00000010;
}

//Controls Rows 
void SetStates (byte statesx){
    // Set latchx to low
    PORTB = B00000000;
    shiftOut(dataPinx, clockPinx, LSBFIRST, statesx);  
    // Set latchx to high
    PORTB = B00010000; 
}

//debugger function to print bytes
void printBits(byte myByte){
 for(byte mask = 0x80; mask; mask >>= 1){
   if(mask  & myByte)
       Serial.print('1');
   else
       Serial.print('0');
 }
}
     

