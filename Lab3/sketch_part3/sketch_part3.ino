 // Pin definitions 
 #define outputA 5
 #define outputB 4
 #define bluePin 9
 #define greenPin 10
 #define redPin 11
 #define echo_pin 12
 #define trig_pin 13
 #define photo_pin 0
 #define range 128
 // Debouncer
 long t1 = 0;
 long delayT = 100;

 // Encoder Change of State
 int counter = 0; 
 int aState;
 int aLastState; 
 int bState; 

int red = 0;
int green = 0; 
int bleu = 0;

 //photo pin
int raw_voltage = 0;
float adj_voltage = 0;
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
   aLastState = digitalRead(outputA); 
   Serial.begin (9600);
 } 
 void loop() { 
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  bState = digitalRead(outputB);
     if (aState != aLastState){
        delay(7);
        aState = digitalRead(outputA);
        if (aState != aLastState) {
         // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
         if (aState != bState) { 
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
       color = map(counter,-128,128,0, 256);
       Serial.println(color);
       Serial.println(getR(color, factor));
       red = getR(color, factor);
       green = getG(color, factor);
       bleu = getB(color, factor);
        }
     }
     factor = intensity();
     /*Serial.print(factor* red);
     Serial.print(" ");
     Serial.print(factor* green);
     Serial.print(" ");
     Serial.println(factor* bleu);*/
     //set the color of the RGB LED
     setColor(255-3*factor*red,255-3*factor*green,255-3*factor*bleu);
     aLastState = aState; // Updates the previous state of the outputA with the current state
 }

//return intensity factor from distance echo sensor
float intensity(){
  raw_voltage = analogRead(photo_pin);
  raw_voltage = map(raw_voltage, 1024,0,0,1024);
  //Serial.println(raw_voltage);
  //Serial.println(raw_voltage/1024.0);
  return raw_voltage/1024.0;
}

int getR(int b, float intensity){
  float r = b * 23 % 256;
  //Serial.println(r);
  return r;
}
int getG(int b, float intensity){
  float g  = b * 31 % 256;
  //Serial.println(g);
  return g;
}

//get a number for 
int getB(int b, float intensity){
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
