/* Anen_Solar_HDC3022_xxx
   Version 001: Basic Influxdb reporting.
           002: Adding the anenometer function.
           004: Configured for ESP32-S2
*/
//
char CODENAME[] = __FILE__;
char DEVNAME[]  = "Anenometer_017";
char LOCNAME[]  = "Outdoor";
float secSleep  = 4.*60.;

#include "arduino_secrets.h" 
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

#if defined(ESP32)
  #include <WiFiMulti.h>
  WiFiMulti wifiMulti;
  #define DEVICE "ESP32"
  #elif defined(ESP8266)
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #define DEVICE "ESP8266"
#endif
  
  #include <InfluxDbClient.h>
  #include <InfluxDbCloud.h>
  
  
  #define INFLUXDB_BUCKET "HouseData"  

  // Time zone info
  #define TZ_INFO "UTC-6"
  
  // Declare InfluxDB client instance with preconfigured InfluxCloud certificate
  InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
  
  // Declare Data point
  Point sensor("Environment_Study");

// Board Definitions
// ESP32-C6
#if defined(ARDUINO_ADAFRUIT_FEATHER_ESP32C6)
const byte interruptPin = 14;
const byte powerPin     = 15;
#endif

// ESP32-S2
#if defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
const byte interruptPin = 12;
const byte powerPin     = 13;
#endif
// ESP32-S3
//const byte interruptPin = 12;
//const byte powerPin     = 13;

// Sensor Includes and Definitions
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>

#include "Adafruit_LC709203F.h"
Adafruit_LC709203F lc;

// RPM Stuff
#include <SoftTimers.h>
SoftTimer anenom_timer;
int iRotationCount = 0;

#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

#include <Adafruit_HDC302x.h>
Adafruit_HDC302x hdc = Adafruit_HDC302x();

#include <Adafruit_NeoPixel.h>
#if defined(PIN_NEOPIXEL)
  Adafruit_NeoPixel pixel(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
#endif



 // SETUP Function =================================================== 
void setup() {

  pinMode(interruptPin,INPUT);
  pinMode(powerPin, OUTPUT);

  Serial.begin(115200);
  delay(1000);
  Serial.print("Awake: ");
  Serial.println(CODENAME);

  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Accurate time is necessary for certificate validation and writing in batches
  // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");


  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
    // Add tags to the data point
  sensor.addTag("DeviceType", DEVICE);
  sensor.addTag("DeviceName", DEVNAME);
  sensor.addTag("SSID",       WiFi.SSID());
  sensor.addTag("Location",   LOCNAME);
  sensor.addTag("CodeName",   CODENAME);

  Wire.begin();
  initializeLC7902903F();
  initializeHDC3022();
  initializeINA219();

  Serial.print("interruptPin   = "); Serial.println(interruptPin);
  Serial.print("powerPin       = "); Serial.println(powerPin);
#if defined(TFT_I2C_POWER) 
  Serial.print("TFT_I2C_POWER  = "); Serial.println(TFT_I2C_POWER); 
#endif
#if defined(PIN_I2C_POWER) 
  Serial.print("PIN_I2C_POWER  = "); Serial.println(PIN_I2C_POWER);
#endif
#if defined(NEOPIXEL_POWER) 
  Serial.print("NEOPIXEL_POWER = "); Serial.println(NEOPIXEL_POWER);
#endif
  Serial.println("Setup Complete");
}
  
// LOOP Function ===================================================   
// LOOP Function ===================================================   
// LOOP Function ===================================================   
  void loop() {
    Serial.println("Starting Loop");
    
    enableInternalPower();
    LEDon();
    delay(500);

    reportPanelPower();
    printHDC3022Report();


    float fWindSpeed = getWindSpeed(60.);

    // Clear fields for reusing the point. Tags will remain the same as set above.
    sensor.clearFields();
  
    // Store measured value into point
    sensor.addField("rssi", WiFi.RSSI());
    sensor.addField("temperature",        getTemperatureF());
    sensor.addField("humidity",           getRelHumidity());
    sensor.addField("windRPM",            fWindSpeed);
    sensor.addField("panelVoltage",       getPanelVoltage());
    sensor.addField("panelCurrent",       getPanelCurrent());
    sensor.addField("panelPower",         getPanelPower());
    sensor.addField("sleepInSeconds",     secSleep);
    sensor.addField("batteryVoltage",     getBatteryVoltage());

  
    // Print what are we exactly writing
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());
  
    // Check WiFi connection and reconnect if needed
    if (wifiMulti.run() != WL_CONNECTED) {
      Serial.println("Wifi connection lost");
    }
  
    // Write point
    if (!client.writePoint(sensor)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    } else {
      Serial.println("Write Successful.");
    }

    disableInternalPower();

    Serial.println("Delay 10sec for programming.");
    delay(10000);

    Serial.print("Deep Sleeping for ");
    Serial.print(secSleep);
    Serial.println(" seconds");
    ESP.deepSleep(secSleep*1e6); 
    //Never Get Here
    Serial.println("Waiting 10 second");
    //delay(10000);
  }

// = = = = = = ==============================================================================
//             timerReset
// Common function to reset the timers.
void timerReset(SoftTimer &inTimer, float inSec) {
  inTimer.setTimeOutTime(1);
  inTimer.reset();
  delay(2);
  inTimer.setTimeOutTime(inSec*1000); //Resample for time average 
  inTimer.reset();
}

