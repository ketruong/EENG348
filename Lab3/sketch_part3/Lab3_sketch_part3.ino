int bluePin = 9;
int greenPin = 10;
int redPin = 11;
int brightness = 120;    // how bright the LED is, start at half brightness
int fadeAmount = 10;    // how many points to fade the LED by
unsigned long currentTime;
unsigned long loopTime;
const int pin_A = 7;  // pin 7
const int pin_B = 6;  // pin 6
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;

void setup() {
  pinMode(bluePin, OUTPUT); //set bluePin as an Output 
  pinMode(greenPin, OUTPUT); //set greenPin as an Output
  pinMode(redPin, OUTPUT);  // set redPin as an Output
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
  currentTime = millis();
  loopTime = currentTime; 
  Serial.begin(9600);
}

void loop() {
   // get the current elapsed time
  currentTime = millis();
  if(currentTime >= (loopTime + 5)){
    // 5ms since last check of encoder = 200Hz  
    encoder_A = digitalRead(pin_A);    // Read encoder pins
    encoder_B = digitalRead(pin_B);   
    if((!encoder_A) && (encoder_A_prev)){
      // A has gone from high to low 
      if(encoder_B) {
        // B is high so clockwise
        // increase the brightness, dont go over 255
        if(brightness + fadeAmount <= 255) brightness += fadeAmount;               
      }   
      else {
        // B is low so counter-clockwise      
        // decrease the brightness, dont go below 0
        if(brightness - fadeAmount >= 0) brightness -= fadeAmount;               
      }   

    }   
    encoder_A_prev = encoder_A;     // Store value of A for next time    
    Serial.println(brightness);
    setColor(brightness, brightness, brightness);    
    loopTime = currentTime;  // Updates loopTime
    delay(1000);
  }
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
