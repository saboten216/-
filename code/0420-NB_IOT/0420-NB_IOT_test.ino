void setup() {
  // initialize both serial ports:
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(2, OUTPUT);
}

void loop() {
  // read from port 2, send to port 0:
  if (Serial2.available()) {
    String inByte = Serial2.readString();
    Serial.println(inByte);
    if(inByte.substring(inByte.length()-4 ,inByte.length()-2 ) == "OK"){
      digitalWrite(2, HIGH);
    }else if (inByte.substring(inByte.length()-7 ,inByte.length()-2) != "ERROR" && inByte.substring(inByte.length()-4 ,inByte.length()-2)!= "OK"){
      digitalWrite(2, LOW);
    }
  }

  // read from port 0, send to port 2:
  if (Serial.available()) {
    String inByte = Serial.readString();
    Serial2.println(inByte);
  }
}
