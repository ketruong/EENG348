int clockPin = 10;
int latchPin = 9;
int dataPin = 8;

int clockPinx = 13;
int latchPinx = 12;
int dataPinx = 11;

int potPin = 2;
int val = 0;
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
  pinMode(latchPinx, OUTPUT);
  pinMode(clockPinx, OUTPUT);
  pinMode(dataPinx, OUTPUT);
  
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, B11111111);  
    digitalWrite(latchPin, HIGH);
    
    Serial.begin(9600);
}

void loop() {
  for(int k =0; k<=9;k++){
  for(int j =0; j<=1000; j++){
    for(int i=0;i<=3;i++){
      SetStates(ledStates[k][i]);
      GroundCorrectLED (GroundLEDs[k][i]);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, B11111111);  
      digitalWrite(latchPin, HIGH);
    }
  }
  delay(1000);
  }
}

void GroundCorrectLED (byte states){
  
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, states);  
    digitalWrite(latchPin, HIGH);
    
}

void SetStates (byte statesx){
       
    digitalWrite(latchPinx, LOW);
    shiftOut(dataPinx, clockPinx, LSBFIRST, statesx);  
    digitalWrite(latchPinx, HIGH);
    
}
     

