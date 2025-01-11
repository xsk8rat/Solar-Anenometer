
void LEDon() {
#if defined(PIN_NEOPIXEL)
  pixel.begin(); // INITIALIZE NeoPixel
  pixel.setBrightness(255); // not so bright
  pixel.setPixelColor(0, 0,0,10);
  pixel.show();
#endif
}

void LEDoff() {
#if defined(PIN_NEOPIXEL)
  pixel.setPixelColor(0, 0x0);
  pixel.show();
#endif
}

void enableInternalPower() {
#if defined(NEOPIXEL_POWER)
  Serial.print("ON         NEOPIXEL_POWER = ");
  Serial.println(NEOPIXEL_POWER);
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
#endif

#if defined(NEOPIXEL_I2C_POWER)
  Serial.print("ON         NEOPIXEL_I2C_POWER = ");
  Serial.println(NEOPIXEL_I2C_POWER);
  pinMode(NEOPIXEL_I2C_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_I2C_POWER, HIGH);
#endif
#if defined(TFT_I2C_POWER)
  Serial.print("ON         TFT_I2C_POWER = ");
  Serial.println(TFT_I2C_POWER);
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
#endif
  
#if defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
  // turn on the I2C power by setting pin to opposite of 'rest state'
  Serial.print("ON          ARDUINO_ADAFRUIT_FEATHER_ESP32S2 = ");
  Serial.println(ARDUINO_ADAFRUIT_FEATHER_ESP32S2);
  pinMode(PIN_I2C_POWER, OUTPUT);
  digitalWrite(PIN_I2C_POWER, HIGH);
  // Turn Neo Pixel Power
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
#endif
}

void disableInternalPower() {
#if defined(NEOPIXEL_POWER)
  Serial.println("OFF         NEOPIXEL_POWER");
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, LOW);
#endif

#if defined(NEOPIXEL_I2C_POWER)
  Serial.println("OFF        NEOPIXEL_I2C_POWER");
  pinMode(NEOPIXEL_I2C_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_I2C_POWER, LOW);
#endif

#if defined(TFT_I2C_POWER)
  Serial.println("OFF         TFT_I2C_POWER");
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, LOW);
#endif

#if defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
  int iUsePower=PIN_I2C_POWER;
  // turn OFF the I2C power by setting pin to rest state LOW
  Serial.println("OFF         ARDUINO_ADAFRUIT_FEATHER_ESP32S2");
  pinMode(PIN_I2C_POWER, OUTPUT);
  digitalWrite(PIN_I2C_POWER, LOW);
  pinMode(NEOPIXEL_POWER, INPUT);
#endif
}