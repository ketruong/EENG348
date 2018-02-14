int clockPin = 4;
int latchPin = 3;
int dataPin = 2;
int clockPinx = 7;
int latchPinx = 6;
int dataPinx = 5;

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
    DDRD = B11111100;
    PORTD = B00000000;
    shiftOut(dataPin, clockPin, LSBFIRST, B11111111); 
    PORTD = B00001000; 
}

void loop() {
  for(int k =0; k<=9;k++){
  for(int j =0; j<=1000; j++){
    for(int i=0;i<=3;i++){
      SetStates(ledStates[k][i]);
      GroundCorrectLED (GroundLEDs[k][i]);
       PORTD = B00000000;
       shiftOut(dataPin, clockPin, LSBFIRST, B11111111); 
       PORTD = B00001000; 
    }
  }
  delay(100);
  }
}

void GroundCorrectLED (byte states){
    PORTD = B00000000;
    shiftOut(dataPin, clockPin, LSBFIRST, states); 
    PORTD = B00001000; 
}

void SetStates (byte statesx){
    PORTD = B00000000;
    shiftOut(dataPinx, clockPinx, LSBFIRST, statesx);  
    PORTD = B01000000;   
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
     

