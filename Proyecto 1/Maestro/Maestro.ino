void setup() {
  Serial3.begin(9600);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char data = Serial.read();
  }
}
