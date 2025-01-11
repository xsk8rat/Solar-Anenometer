void initializeLC7902903F() {
  if (!lc.begin()) {
    Serial.println(F("Couldnt find Adafruit LC709203F?\nMake sure a battery is plugged in!"));
    while (1) delay(10);
  }
  Serial.println(F("Found LC709203F"));
  Serial.print("Version: 0x"); Serial.println(lc.getICversion(), HEX);

  lc.setPackSize(LC709203F_APA_3000MAH);

}

float getBatteryVoltage() {
  float fBatteryVoltage;
    // Averaging two readings
    fBatteryVoltage = lc.cellVoltage();
    delay(500);
    fBatteryVoltage = fBatteryVoltage + lc.cellVoltage();
    fBatteryVoltage = fBatteryVoltage/2.;
    //Serial.print("Battery Voltage = ");
    //Serial.println(fBatteryVoltage);
    return(fBatteryVoltage);
}