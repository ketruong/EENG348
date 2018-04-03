int raw_voltage = 0;
int new_voltage = 0;
int photo_in = 0;
int bluePin = 9;
int greenPin = 10;
int redPin = 11;
void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(photo_in, INPUT);
  Serial.begin(9600);
}

void loop() {
  raw_voltage = analogRead(photo_in);
  raw_voltage = map(raw_voltage, 1023,0,0,1024);
  Serial.print("Adjusted voltage: ");
  new_voltage = map(raw_voltage, 0, 1023, 0, 255);
  Serial.println(new_voltage);
  new_voltage = map(raw_voltage, 0, 255, 255, 0); // need to re-map because of our LED type
  setColor(new_voltage, new_voltage, new_voltage); 
  delay(1000);
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
