long lastDebounceTime = 0; // time of last debounce  
long debounceDelay = 50;   // time for button to settle
int buttonState = 1;       // records current state

void setup(){
  DDRD = B0000000;
  Serial.begin(9600);
}

void loop(){
  // read portD registers to see if button is pressed
  byte pinD = PIND;
  byte mask = 0x4;

  //check if button has settled 
  if ((millis() - lastDebounceTime) > debounceDelay) {
    
    // check from off to on
    if ((mask & pinD) != 0 & buttonState == 0) {
        Serial.println("Button Released!");
       // change state to released
        buttonState = 1;
       
     // check from on to off
    } else if ((mask & pinD) == 0 & buttonState != 0) {
        Serial.println("Button Pressed!");
       // change state to pressed
        buttonState = 0;
    }

    // update time
    lastDebounceTime = millis();
  }
}

//debugger function to print bytes
void printBits(byte myByte){
  byte mask = 0x4;
  if (mask  & myByte) {
      Serial.print('1');
  } else {
      Serial.print('0');
  }
  Serial.print('\n');
}
