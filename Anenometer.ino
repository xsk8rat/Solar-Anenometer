void attachTheInterupt() {
  attachInterrupt(digitalPinToInterrupt(interruptPin), computeRPM, FALLING);
  int checkPin=digitalPinToInterrupt(interruptPin);
  if (checkPin == -1) {
    Serial.println("Unable to attach the interrupt to the pin.");
    Serial.println(checkPin);
  }
}

void detachTheInterupt() {
  detachInterrupt(digitalPinToInterrupt(interruptPin));
}

ICACHE_RAM_ATTR void computeRPM() {
  iRotationCount++;
  Serial.println(iRotationCount);
}

float getWindSpeed (float inSecToSample) {
  Serial.println("In getWindSpeed");
  interPowerON();
  if (inSecToSample <= 0) {
    return 0.0;
  };
  float rpm;
  float minutesToCount = inSecToSample/60.;
  SoftTimer anenom_timer; //millisecond timer
  timerReset(anenom_timer,  inSecToSample);
  iRotationCount = 0;
  // Allow Interupts
  Serial.println("Attaching Interupt");
  attachTheInterupt();
  while (!anenom_timer.hasTimedOut()) {
    //interupts will count the rising edges
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  // Stop Interupts
  detachTheInterupt();
  Serial.println("Detaching Interupt");

  rpm = iRotationCount/(minutesToCount);

  Serial.print("RPM = ");
  Serial.println(rpm);
  interPowerOFF();
  return rpm;
}

void interPowerON() {
   digitalWrite(powerPin, HIGH);
 }

void interPowerOFF() {
   digitalWrite(powerPin, LOW);
 }