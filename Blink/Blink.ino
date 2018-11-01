void setup() {
  Serial.begin(9600);
  pinMode(D5,OUTPUT);
}

void loop() {
  digitalWrite(D5,HIGH);
  Serial.println("HIGH");
  delay(1000);
  digitalWrite(D5,LOW);
  Serial.println("LOW");
  delay(1000);
}
