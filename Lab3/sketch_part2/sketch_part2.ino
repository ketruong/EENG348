int bluePin = 9;
int greenPin = 10;
int redPin = 11;
int echo_pin = 12;
int trig_pin = 13;

unsigned long duration;
unsigned long distance;

void setup() {
  pinMode(bluePin, OUTPUT); // blue out
  pinMode(greenPin, OUTPUT); // green out
  pinMode(redPin, OUTPUT);  // red out
  pinMode(trig_pin, OUTPUT); // trig out
  pinMode(echo_pin, INPUT); // echo in
  Serial.begin(9600);
}

void loop() {
  // send a pulse
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  // read the echoPin, return the travel time in ms
  duration = pulseIn(echo_pin, HIGH);
  
  // get distance in centimeters
  distance = duration*0.034/2; 
  int volt = map(distance, 400, 0, 0, 255);
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  //Serial.print("Volt: ");
  //Serial.println(volt);
  setColor(volt, volt, volt); 
  delay(1000);
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
