int bluePin = 9;
int greenPin = 10;
int redPin = 11;
int echo_pin = 12;
int trig_pin = 13;

long duration;
int distance;

void setup() {
  pinMode(bluePin, OUTPUT); //set bluePin as an Output 
  pinMode(greenPin, OUTPUT); //set greenPin as an Output
  pinMode(redPin, OUTPUT);  // set redPin as an Output
  pinMode(trig_pin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo_pin, INPUT); // Sets the echoPin as an Input  
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo_pin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  int volt = map(distance, 0, 1024, 0, 255);
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
   Serial.print("Volt: ");
  Serial.println(volt);
  delay(1000);
  setColor(volt, volt, volt); 
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
