/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

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
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDr5vNs0qaT1YOj\nlBEbmul0Py9dG0vzaWC57aAP4S+yfkBajtxJ4opFZztoIOZWcVrngFy5ZuyHnEVY\nl3zmO/X9cRxrhfuPd89ePnxWyDjTw2pFUad9WW61RlJktfdTCP05Qn8Fe0G/ghvH\nVuBC6savKZgJ3lZ1OmYJnh3k5yiQkyDg9KImV/o5RlxGa1puwtsSFQolTKdWftIZ\nt5/A0ZcsCuGUN4ztUrfnSQvkjKuxzScAKsx+BYg4VweWKdDM0Y2XqQlhD6ZqzrY0\n+2xymQM4ANP7qLZUoJ/5QDa2IvXS8WJVjCPkZ8gTJam8M6ng1POLRAcin61yqbx7\n2OaTuQynAgMBAAECggEACtq4FahFqOS4p2SvOtVwa3xiygZxArS1EikIZeKnJq/f\ndys+oJmvYg0jcyGcZ5rUo3VJDvDY7k529GIYCaH1qiEzvRDPp+TL4lq+ruGMl9XU\nhHcC3WYpjWa8cMzcofH2cNWJ8a8fsdejPpNVQ075MGMaD04vLTqwKj4fwwIN4EUG\nLRByuIlm4wt9XqyECVHzOsVUwQbzcIQId65ce94XL/r8OKqo990dgy2B2T0dffBn\nc5Kb7fdJkIbHOUgfVEl32sA8bXu83b4ETZcqez8nPh/Dj6PNrrNvUohkcyyo7s9/\nnMySvrp9TCC0odcV1se4/zA1Fs7LmOgy4YKh2OxZaQKBgQD8vCkRmd9gA0TgScxZ\nYTF+St9WPWXmZ3XSanRtfeQbzK5M5nN6pQXCEmY6xyWG/5sHLSUDmPAOtAmDxJA4\nNLNzBdGCLx6T0u2F/yJKGOBGsXfAoD+AQerHlboreClxWmjdSjXF0BrpTj7KpApo\nXPFbqwyK0fzzC+ZfitdnFvqiKQKBgQDu8x8Izajp+VvniNWdspqyVVGA8Ene/UHh\nMHG/QItWxuBHHVPfIZ++Pj0ZovIC8iFd7T/62VmZn3Cj3IWi6qcG5eFGnTEuUCdO\nuIvU2VlxaaDUleo5THiiWySr0JQaA++sXiLq5N3eP/TbZcdmSTi4GdfJJLZjqg88\nBuhWu60yTwKBgQDkcZeVp0Y3XP0W1t31NFsuUrMJGrpOxtnC9iKDDbXwQtSSAgQD\nqcJIp6TsIDIVklko5W99jvH9+FJZXoUVqN1DbNDxIXI8H/4+jl0KKO4gOC2MWczM\nO8jh2r7Kp/Sd1yDOVVPBl9ebZvRu6lPV/ZzURQozOJexgOIdHjyRnD2z8QKBgG7w\n+kvAENjxKbaKVYPuVNzPuBPBfMrHkzrWnzlI4mjb1W2vNC25Es3lX8DzYqHzgYJ2\nkOoK2Ar3d/jcfj9x6zLMsJd0jE0Qce+ex21/rIN3o3CQWjtYQfo/jvsm0Fxzz2ec\nHWIp+MkjsK/cxYO8I1V2yjVdkkEbyA76tgzu/i8JAoGBAIhpclpbgAa0yLtJpkDL\nOepRCA4h90RDnFpsVMFJH5YBRvtB2AcHKRgjFwcVGyqzVaYdniaXk97jUjpSUxtV\nIaogpVEDN9yY6fnFkYq6XLxnwjNaVGlZ0/xclhWnAF+byyUqGfAM4a2xccnYUdIk\nS64DvWio0kOHf06hl9rEc4K+\n-----END PRIVATE KEY-----\n";
bool taskComplete = false;
void tokenStatusCallback(TokenInfo info);


void setup() {

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
  while (WiFi.status() != WL_CONNECTED)
  {
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


void tokenStatusCallback(TokenInfo info)
{
  if (info.status == esp_signer_token_status_error)
  {
    Serial.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    Serial.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  }
  else
  {
    Serial.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}
