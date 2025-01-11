void initializeINA219() {
  ina219.begin(); 
  delay(500);
  ina219.setCalibration_16V_400mA();

}

void reportPanelPower() {
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:     "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage:   "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:    "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:         "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:           "); Serial.print(power_mW);            Serial.println(" mW");
  Serial.print("Battery Voltage: "); Serial.print(getBatteryVoltage()); Serial.println(" V");
  Serial.println("");
}

float getPanelVoltage() {
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage   = ina219.getBusVoltage_V();
  float loadvoltage  = busvoltage + (shuntvoltage / 1000);
  return(loadvoltage);
}

float getPanelCurrent() {
  float current_mA = ina219.getCurrent_mA();
  return(current_mA);
}

float getPanelPower() {
  float power_mW = ina219.getPower_mW();
  return(power_mW);
}
