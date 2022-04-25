void setup() {
    // initialize both serial ports:
    Serial.begin(115200);
    Serial2.begin(115200);
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
}

void loop() {
    
    Serial2.println("AT"); //return OK or ERROR
    check(Serial2.readString());
    Serial2.println("AT+CGDCONT=1,\"IP\",\"internet.iot\""); // return OK or ERROR
    check(Serial2.readString());
    Serial2.println("AT+COPS=1,2,\"46692\""); // return OK or ERROR
    check(Serial2.readString());
//    Serial2.println("AT+CGATT?");
//    check(Serial2.readString());
//    Serial2.println("AT+CPSI?");
//    check(Serial2.readString());
    Serial2.println("AT+CSTT= \"internet.iot\" "); // return OK or ERROR
    check(Serial2.readString());
    Serial2.println("AT+CIICR"); // return OK or ERROR
    check(Serial2.readString());
    Serial2.println("AT+CIPPING=\"8.8.8.8\""); // return ERROR or info
    check(Serial2.readString());
    Serial.println("done");  
}

void check(String inByte){
    delay(3000);
    Serial.println(inByte);
    if(inByte.substring(inByte.length()-4 ,inByte.length()-2 ) == "OK"){
      digitalWrite(2, HIGH);
    }else if (inByte.substring(inByte.length()-7 ,inByte.length()-2) != "ERROR" && inByte.substring(inByte.length()-4 ,inByte.length()-2)!= "OK"){
      digitalWrite(2, LOW);
    }
}
