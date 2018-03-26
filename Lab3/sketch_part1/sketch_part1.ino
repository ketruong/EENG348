int photo_pin = 0;
int bluePin = 9;
int greenPin = 10;
int redPin = 11;
int raw_voltage = 0;
int adj_voltage = 0;
void setup() {
  pinMode(photo_pin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
  Serial.begin(9600);
}

void loop() {
  raw_voltage = analogRead(photo_pin);
  raw_voltage = map(raw_voltage, 1024,0,0,1024);
  Serial.print("Raw voltage: ");
  Serial.println(raw_voltage);
  Serial.print("Adjusted voltage: ");
  adj_voltage = map(raw_voltage, 0, 1024, 0, 255);
  Serial.println(adj_voltage);
  setColor(adj_voltage, adj_voltage, adj_voltage); 
  delay(1000);
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
