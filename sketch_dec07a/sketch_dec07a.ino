void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(D0, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World! On!");
  digitalWrite(D0, HIGH);
  delay(5000);
  Serial.println("Hello World! Off!");
  digitalWrite(D0, LOW);
  delay(5000);
}
