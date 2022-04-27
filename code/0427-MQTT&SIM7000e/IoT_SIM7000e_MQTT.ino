  
  #include "DHT.h"
  #define DHTPIN 4
  #define DHTTYPE DHT22
  DHT dht(DHTPIN, DHTTYPE);
  
  
  #include "Adafruit_FONA.h"
  #define SIMCOM_7000
  
  #define COUNTER(func, acttime, lasttime) \
    if (millis() - lasttime >= acttime)    \
    {                                      \
      func;                                \
      lasttime = millis();                 \
    }
  
  /************************* PIN DEFINITIONS *********************************/
  // For botletics SIM7000 shield with ESP32
  #define FONA_TX 16 // ESP32 hardware serial RX2 (GPIO16)
  #define FONA_RX 17 // ESP32 hardware serial TX2 (GPIO17)
  
  
  #define LED 2 // Just for testing if needed!
  
  #define samplingRate 30
  
  // For ESP32 hardware serial
  #include <HardwareSerial.h>
  HardwareSerial fonaSS(1);
  
  Adafruit_FONA_LTE fona = Adafruit_FONA_LTE();
  
  /************************* MQTT PARAMETERS *********************************/
  #define MQTT_SERVER "xxx.xx.xx.xxx"
  #define MQTT_PORT 1883
  #define MQTT_USERNAME "xxxxx"
  #define MQTT_PASSWORD "xxxxx"
  
  // Set topic names to publish and subscribe to
  
  #define PUB_TOPIC "xxxxxxxx/xxxxxxxx"
  #define SUB_TOPIC "command" // Subscribe topic name
  
  uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
  char imei[16] = {0}; // Use this for device ID
  uint8_t type;
  uint16_t battLevel = 0; // Battery level (percentage)
  uint8_t counter = 0;
  
  char latBuff[12], longBuff[12], locBuff[50], speedBuff[12],
       headBuff[12], altBuff[12], tempBuff[12], pubBuff[12];
  
  void setup()
  {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println(F("*** SIM7000 MQTT Example ***"));
    dht.begin();
  
  #ifdef LED
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
  #endif
  
    moduleSetup(); // Establishes first-time serial comm and prints IMEI
  
    // Set modem to full functionality
    fona.setFunctionality(1); // AT+CFUN=1
  
    fona.setNetworkSettings(F("internet.iot")); // For Hologram SIM card
  
    while (!netStatus())
    {
      Serial.println(F("Failed to connect to cell network, retrying..."));
      delay(2000); // Retry every 2s
    }
    Serial.println(F("Connected to cell network!"));
  
    // Open wireless connection if not already activated
    if (!fona.wirelessConnStatus())
    {
      while (!fona.openWirelessConnection(true))
      {
        Serial.println(F("Failed to enable connection, retrying..."));
        delay(2000); // Retry every 2s
      }
      Serial.println(F("Enabled data!"));
    }
    else
    {
      Serial.println(F("Data already enabled!"));
    }
  
    // If not already connected, connect to MQTT
    if (!fona.MQTT_connectionStatus())
    {
      // Set up MQTT parameters (see MQTT app note for explanation of parameter values)
      fona.MQTT_setParameter("URL", MQTT_SERVER, MQTT_PORT);
      fona.MQTT_setParameter("CLIENTID", "sim7000e");
      // Set up MQTT username and password if necessary
      fona.MQTT_setParameter("USERNAME", MQTT_USERNAME);
      fona.MQTT_setParameter("PASSWORD", MQTT_PASSWORD);
      //    fona.MQTTsetParameter("KEEPTIME", 30); // Time to connect to server, 60s by default
  
      Serial.println(F("Connecting to MQTT broker..."));
      if (!fona.MQTT_connect(true))
      {
        Serial.println(F("Failed to connect to broker!"));
      }
    }
    else
    {
      Serial.println(F("Already connected to MQTT server!"));
    }
    fona.MQTT_unsubscribe(SUB_TOPIC);
    fona.MQTT_subscribe(SUB_TOPIC, 1); // Topic name, QoS
  }
  
  int PUB_time = millis();
  int Measure_time = millis();
  float tempC, tempF;
  
  void loop()
  {
    // put your main code here, to run repeatedly:
  
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);
  
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
  
    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
  
    float temperature = t ; // Select what unit you want to use for this example
  
    // Format the floating point numbers
    // float_val, min_width, digits_after_decimal, char_buffer
    dtostrf(temperature, 3, 1, pubBuff); //temperature 是變數
  
    // Now publish all the GPS and temperature data to their respective topics!
    // Parameters for MQTT_publish: Topic, message (0-512 bytes), message length, QoS (0-2), retain (0-1)
    delay(2000);
    fona.MQTT_publish(PUB_TOPIC, pubBuff, strlen(pubBuff), 1, 0);
  
  
    // Unsubscribe to topics if wanted:
  
    // Enable MQTT data format to hex
    //  fona.MQTT_dataFormatHex(true); // Input "false" to reverse
  
    // Disconnect from MQTT
    //  fona.MQTT_connect(false);
  
    // Delay until next post
    //  Serial.print(F("Waiting for "));
    //  Serial.print(samplingRate);
    //  Serial.println(F(" seconds\r\n"));
    //  delay(samplingRate * 1000UL); // Delay
    //  fona.MQTT_unsubscribe(SUB_TOPIC);
  
    // flush input
    // while (Serial.available())
    //   Serial.read();
    // while (fona.available())
    // {
    //   Serial.write(fona.read());
    // }
  }
  
  void moduleSetup()
  {
    // SIM7000 takes about 3s to turn on and SIM7500 takes about 15s
    // Press Arduino reset button if the module is still turning on and the board doesn't find it.
    // When the module is on it should communicate right after pressing reset
  
    // Software serial:
    fonaSS.begin(9600, SERIAL_8N1, FONA_TX, FONA_RX); // baud rate, protocol, ESP32 RX pin, ESP32 TX pin
  
    Serial.println(F("Configuring to 115200 baud"));
    fonaSS.println("AT+IPR=115200");                    // Set baud rate
    delay(100);                                       // Short pause to let the command run
    fonaSS.begin(115200, SERIAL_8N1, FONA_TX, FONA_RX); // Switch to 9600
    if (!fona.begin(fonaSS))
    {
      Serial.println(F("Couldn't find FONA"));
      while (1)
        ; // Don't proceed if it couldn't find the device
    }
  
    type = fona.type();
    Serial.println(F("FONA is OK"));
    Serial.print(F("Found "));
    switch (type)
    {
      case SIM800L:
        Serial.println(F("SIM800L"));
        break;
      case SIM800H:
        Serial.println(F("SIM800H"));
        break;
      case SIM808_V1:
        Serial.println(F("SIM808 (v1)"));
        break;
      case SIM808_V2:
        Serial.println(F("SIM808 (v2)"));
        break;
      case SIM5320A:
        Serial.println(F("SIM5320A (American)"));
        break;
      case SIM5320E:
        Serial.println(F("SIM5320E (European)"));
        break;
      case SIM7000:
        Serial.println(F("SIM7000"));
        break;
      case SIM7070:
        Serial.println(F("SIM7070"));
        break;
      case SIM7500:
        Serial.println(F("SIM7500"));
        break;
      case SIM7600:
        Serial.println(F("SIM7600"));
        break;
      default:
        Serial.println(F("???"));
        break;
    }
  
    // Print module IMEI number.
    uint8_t imeiLen = fona.getIMEI(imei);
    if (imeiLen > 0)
    {
      Serial.print("Module IMEI: ");
      Serial.println(imei);
    }
  }
  
  // Read the module's power supply voltage
  float readVcc()
  {
    // Read battery voltage
    if (!fona.getBattVoltage(&battLevel))
      Serial.println(F("Failed to read batt"));
    else
      Serial.print(F("battery = "));
    Serial.print(battLevel);
    Serial.println(F(" mV"));
  
    // Read LiPo battery percentage
    //  if (!fona.getBattPercent(&battLevel)) Serial.println(F("Failed to read batt"));
    //  else Serial.print(F("BAT % = ")); Serial.print(battLevel); Serial.println(F("%"));
  
    return battLevel;
  }
  
  bool netStatus()
  {
    int n = fona.getNetworkStatus();
  
    Serial.print(F("Network status "));
    Serial.print(n);
    Serial.print(F(": "));
    if (n == 0)
      Serial.println(F("Not registered"));
    if (n == 1)
      Serial.println(F("Registered (home)"));
    if (n == 2)
      Serial.println(F("Not registered (searching)"));
    if (n == 3)
      Serial.println(F("Denied"));
    if (n == 4)
      Serial.println(F("Unknown"));
    if (n == 5)
      Serial.println(F("Registered roaming"));
  
    if (!(n == 1 || n == 5))
      return false;
    else
      return true;
  }
