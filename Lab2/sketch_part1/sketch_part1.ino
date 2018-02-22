long t1 = 0; // time of last debounce  
long delayT = 50;   // time for button to settle
int buttonState = 1;       // records current state

void setup(){
  DDRD = 0b11111011; // set third pin as output
  PORTD = 0b00000100; // turn internal pullup resistor on for third pin
  Serial.begin(9600);
}

void loop(){
  // read portD registers to see if button is pressed
  byte pinD = PIND;
  byte mask = 0x4; 

  //check if button has settled 
  if ((millis() - t1) > delayT) {

    //for (mask = 0x80; mask >= 0x1; mask = mask >> 0x1) {
      //Serial.print(PIND & mask);
    //}
    //Serial.print("  ");
    //Serial.print("\n");

    //mask = 0x04;

    // check from off to on
    if ((mask & pinD) == 0 & buttonState != 0) {
        Serial.println("Button Pressed!");
       // change state to pressed
        buttonState = 0;
       
     // check from on to off
    } else if ((mask & pinD) != 0 & buttonState == 0) {
        Serial.println("Button Released!");
       // change state to released
        buttonState = 1;
    }

    // update time
    t1 = millis();
  }
}

//debugger function to print bytes
void printBits(byte myByte){
  Serial.print(myByte);
  Serial.print('\n');
}
