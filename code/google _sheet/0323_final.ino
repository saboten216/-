/**
   BasicHTTPClient.ino
   Created on: 24.05.2015
   
   use : https://github.com/mobizt/ESP-Google-Sheet-Client 
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ESP_Google_Sheet_Client.h>
#include "time.h"
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22

WiFiMulti wifiMulti;
DHT dht(DHTPIN, DHTTYPE);

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 3600;

#define WIFI_SSID "test"
#define WIFI_PASSWORD "00123456"
#define PROJECT_ID "mineral-weaver-345002"
#define CLIENT_EMAIL "saboten@mineral-weaver-345002.iam.gserviceaccount.com"
#define USER_EMAIL "C108134127@nkust.edu.tw"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\-----END PRIVATE KEY-----\n";
bool taskComplete = false;
void tokenStatusCallback(TokenInfo info);

void setup(){

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.setAutoReconnect(true);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  randomSeed(analogRead(0));
  pinMode(2, OUTPUT);
  dht.begin();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  GSheet.setTokenCallback(tokenStatusCallback);
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);
}

int i = 1;
void loop() {
  bool ready = GSheet.ready();
  if (ready && !taskComplete) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F("Humidity: "));
    Serial.print(h); //濕度
    Serial.print(F("%  Temperature: "));
    Serial.print(t); //溫度
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));
    Serial.println("\nUpdate spreadsheet values...");
    Serial.println("----------------------------------------------------------------");

    FirebaseJson response;
    FirebaseJson valueRange;

    struct tm timeinfo;
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    getLocalTime(&timeinfo);
    String Hour = String(timeinfo.tm_hour);
    String Min = String(timeinfo.tm_min);
    String Sec = String(timeinfo.tm_sec);
    String Time = Hour+":"+Min+":"+Sec;
    String row = String(i);
    String sheetRange = "Sheet1!A"+ row +":C100";
    String T = String(t);
    String H = String(h);
    String time_row = "values/[" + row + "]/[0]"; //時間
    String t_row = "values/[" + row + "]/[1]";//溫度
    String h_row = "values/[" + row + "]/[2]"; // 濕度
    
    valueRange.add("range", sheetRange);
    valueRange.add("majorDimension", "ROWS");
    valueRange.set(time_row ,Time); //row 2/ column 1
    valueRange.set(t_row, T); //row 2/ column 2
    valueRange.set(h_row, H); //row 2/ column 3
    bool success = GSheet.values.update(&response /* returned response */, "15LLcbrozRGMIZB0sfLaxdf9TLSykYM5XFXMTnMztBZo" /* spreadsheet Id to update */, sheetRange /* range to update */, &valueRange /* data to update */);
   
    response.toString(Serial, true);
    Serial.println();

    Serial.println("\nUpdate spreadsheet values in multiple ranges...");
    Serial.println("----------------------------------------------------------------");
    i = i + 1;
  }
  delay(5000);
}


void tokenStatusCallback(TokenInfo info){
  if (info.status == esp_signer_token_status_error){
    Serial.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    Serial.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  }
  else{
    Serial.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}
