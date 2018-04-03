#define outputA 5
#define outputB 4
#define bluePin 9
#define greenPin 10
#define redPin 11
#define echo_pin 12
#define trig_pin 13
#define photo_pin 0
#define range 128
//photo pin
int raw_voltage = 0;
float new_voltage = 0;

// Debouncer
long t1 = 0;
long delayT = 100;

// Encoder Change of State
int counter = 0; 
int aPulse;
int bPulse;
int aPrev;

int red = 0;
int green = 0; 
int bleu = 0;

int color;
float factor;
void setup() { 
  //set encoder pins 
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  digitalWrite(outputA,HIGH);
  digitalWrite(outputB,HIGH);
  
  // RBG
  pinMode(bluePin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(redPin, OUTPUT);  
  
  //Photo Resistor
  pinMode(photo_pin, INPUT);
  
  // Reads the initial state of the outputA
  aPrev = digitalRead(outputA); 
  Serial.begin (9600);
} 
void loop() { 
  aPulse = digitalRead(outputA); // Reads the "current" state of the outputA
  bPulse = digitalRead(outputB);
    if (aPulse != aPrev) {
      delay(7); // optimal debounce time
      aPulse = digitalRead(outputA);
      if (aPulse != aPrev) {
        // If the bPulse state != aPulse state, the encoder is rotating clockwise
        if (aPulse != bPulse) { 
          counter++;
          if (counter >= 255) {
            counter = 255;
          }
        } else {
          counter--;
          if (counter <= 0) {
            counter = 0;
          }
         
        }
      Serial.print("Position: ");
      Serial.println(counter);
      color = map(counter, 0, 255,0, 255);
      // Serial.println(color);
      // Serial.println(getR(color, factor));
      red = getR(color);
      green = getG(color);
      bleu = getB(color);
      }
    }
    factor = intensity();
    //set the color of the RGB LED
    setColor(255-factor*red,255-factor*green,255-factor*bleu);
    aPrev = aPulse; // update aPrev
 }

// return intensity factor from distance echo sensor
float intensity(){
  raw_voltage = analogRead(photo_pin);
  raw_voltage = map(raw_voltage, 1024,0,0,1024);
  //Serial.println(raw_voltage);
  //Serial.println(raw_voltage/1024.0);
  return raw_voltage/1024.0;
}

int getR(int b){
  float r = b * 23 % 256;
  //Serial.print(r);
  return r;
}
int getG(int b){
  float g  = b * 31 % 256;
  //Serial.print(g);
  return g;
}

int getB(int b){
  float blue = b * 41 % 256;
  //Serial.println(blue);
  return blue;
}

//set color of RGB LED
void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
