/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#define USE_SERIAL Serial
WiFiMulti wifiMulti;

#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 3600;

void setup() {
    
    Serial.begin(115200);
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    wifiMulti.addAP("test", "00123456"); //wifi帳號密碼
    randomSeed(analogRead(0));

    pinMode(2, OUTPUT);
    dht.begin();

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
}
int i = 0;
void loop() {
    
    if((wifiMulti.run() == WL_CONNECTED)) {
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
        struct tm timeinfo;
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        getLocalTime(&timeinfo);
        
        
        String T = String(t);
        String H = String(h);
        String web = "https://script.google.com/macros/s/AKfycby9geGtBvnCeiLNNWd-RSQx4Fi2d5Po2Ow2pYCORIufCqinJvaDeW1ProH5oaSvnaiIag/exec?data=";
        String Hour = String(timeinfo.tm_hour);
        String Min = String(timeinfo.tm_min);
        String Sec = String(timeinfo.tm_sec);

        i= i +1;
        String I = String(i);
        
        String Updata = web +Hour+":"+Min+":"+ Sec + "," + I + "," + h +"," + t;
        Serial.println(Updata);

        HTTPClient http;
        USE_SERIAL.print("[HTTP] begin...\n");
        http.begin(Updata); //HTTP
        USE_SERIAL.print("[HTTP] GET...\n");
        int httpCode = http.GET();

        if(httpCode > 0) {
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    delay(5000);
}
