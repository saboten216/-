void setup()
{
  pinMode(2, OUTPUT);
}


void loop()
{
  delay(1000); // 1秒
  digitalWrite(2, HIGH);
  delay(1000); // 1秒
  digitalWrite(2, LOW);
}
