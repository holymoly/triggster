//used pins 1,2,3,4,5,6,7,8,9,10,11,12,13
//free pins
#define focusPinIn    6
#define shutterPinIn  7

#define focusPinOut    4
#define shutterPinOut  12

#define externalDigPin  A1
#define externalAnaPin  A0

//Setup routine
void setup(void) {
  initMenu();

  pinMode(focusPinIn, INPUT);
  digitalWrite(focusPinIn, HIGH);       // turn on pullup resistor
  pinMode(shutterPinIn, INPUT);
  digitalWrite(shutterPinIn, HIGH);     // turn on pullup resistor

  pinMode(focusPinOut, OUTPUT);
  digitalWrite(focusPinOut, LOW);
  pinMode(shutterPinOut, OUTPUT);
  digitalWrite(shutterPinOut, LOW);

  pinMode(externalDigPin, INPUT);
  digitalWrite(externalDigPin, HIGH);    // turn on pullup resistor
}

void loop() {
  interrupts();
  //Focus Part
  if (!digitalRead(focusPinIn)){
    noInterrupts();
    digitalWrite(focusPinOut, HIGH);
    delay(getFocusDelay()); // waits for a second
    digitalWrite(focusPinOut, LOW);
  }

  //Shutter Menu Part
  if (!digitalRead(shutterPinIn)){
    switch (getActiveMenu()) {
      case 0:
        noInterrupts();
        digitalWrite(shutterPinOut, HIGH);
        delay(getShutterDelay()); // waits for a second
        digitalWrite(shutterPinOut, LOW);
        break;
      case 1:
        noInterrupts();
        delay(getDelayModeDelay());
        digitalWrite(shutterPinOut, HIGH);
        delay(getShutterDelay()); // waits for a second
        digitalWrite(shutterPinOut, LOW);
        break;
      case 2:
        noInterrupts();
        for(int i = 0; i < getIntervalModeAmount(); i++){
          digitalWrite(shutterPinOut, HIGH);
          delay(getShutterDelay()); // waits for a second
          digitalWrite(shutterPinOut, LOW);
          delay(getIntervalModeWait());
        }
        break;
      case 3:
        noInterrupts();
        digitalWrite(shutterPinOut, HIGH);
        delay(getShutterDelay()); // waits for a second
        digitalWrite(shutterPinOut, LOW);

        delay(getBulbModeDuration());

        digitalWrite(shutterPinOut, HIGH);
        delay(getShutterDelay()); // waits for a second
        digitalWrite(shutterPinOut, LOW);
        break;
      case 4:
        noInterrupts();
        for(int i = 0; i < getBulIntervalbModeAmaount(); i++){
          digitalWrite(shutterPinOut, HIGH);
          delay(getShutterDelay()); // waits for a second
          digitalWrite(shutterPinOut, LOW);

          delay(getBulIntervalbModeDuration());

          digitalWrite(shutterPinOut, HIGH);
          delay(getShutterDelay()); // waits for a second
          digitalWrite(shutterPinOut, LOW);
          delay(getBulIntervalbModeWait());
        }
        break;
      case 7:
        //Do nothing in general mode
        break;
    };
  }
  //Works without shutter switch pressed
  switch (getActiveMenu()) {
    case 5:
      noInterrupts();
      if (!digitalRead(externalDigPin)){
        delay(getExternalDigModeDelay());

        digitalWrite(shutterPinOut, HIGH);
        delay(getShutterDelay()); // waits for a second
        digitalWrite(shutterPinOut, LOW);
      }
      break;
    case 6:
      noInterrupts();
      if (analogRead(externalAnaPin) > getExternalAnaModeThreshold()){
        delay(getExternalAnaModeDelay());

        digitalWrite(shutterPinOut, HIGH);
        delay(getShutterDelay()); // waits for a second
        digitalWrite(shutterPinOut, LOW);
      }
      break;
  }
}
