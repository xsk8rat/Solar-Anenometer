void initializeHDC3022() {
  if (! hdc.begin(0x44, &Wire)) {
    Serial.println("Could not find HDC3022 sensor?");
    while (1);
  }
  Serial.println("HDC3022 Initialized.");  
}

void printHDC3022Report() {
  double temp = 0.0;
  double RH = 0.0;
  hdc.readTemperatureHumidityOnDemand(temp, RH, TRIGGERMODE_LP0);

  Serial.print("Temperature  = ");
  Serial.print(temp*(9./5.)+32.);
  Serial.println(" °F");

  Serial.print("Humidity     = ");
  Serial.print(RH);
  Serial.println(" %");
  delay(2000);
  
}

float getTemperatureC() {
  double temp = 0.0;
  double RH = 0.0;
  hdc.readTemperatureHumidityOnDemand(temp, RH, TRIGGERMODE_LP0);
  return( float(temp));
}
float getTemperatureF() {
  double temp = 0.0;
  double RH = 0.0;
  hdc.readTemperatureHumidityOnDemand(temp, RH, TRIGGERMODE_LP0);
  return( 32.0+float(temp*(9./5.)));
}

float getRelHumidity() {
  double temp = 0.0;
  double RH = 0.0;
  hdc.readTemperatureHumidityOnDemand(temp, RH, TRIGGERMODE_LP0);
  return( float(RH));
}