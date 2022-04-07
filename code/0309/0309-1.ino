
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //  Serial.println("good!");
  if (Serial.available()) {
    char c = Serial.read();

    if (c == '0') {
      digitalWrite(2, LOW);
    } else if (c == '1') {
      digitalWrite(2, HIGH);
    }
    Serial.print(c);
  }
  //  delay(1000);
}
