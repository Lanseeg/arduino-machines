//////////PERANS SYSTEMS - BENDY POWER STATION/////////////
////PROJECT TO BRING LIGHT AND USB CHARGING IN A CAMPING///
////SHED. MOTION SENSOR, LIGHT SENSOR, 12V RELAY & LIGHT///
////POWERED BY A BATTERY CHARGED BY A SOLAR PANNEL.     ///
/////////////////////////PERAN.DEV/////////////////////////
///////////////////////////////////////////////////////////
/////////////////////////////////////////HARDWARE ROUTING//
const int LED = 6;
const int motionSensor = 2;
const int relayPin = 8;
const int lightPin = A0;         // Broche du capteur de luminosité
const int lightThreshold = 200;  // Seuil de luminosité pour le jour (à ajuster selon vos besoins)
int relayStatus = 0;
///////////////////////////////////////////TIMER SETTINGS//
unsigned long lastMillis = 0;
const unsigned long onTime = 5000;
/////////////////////////////////////////TIMER SETTINGS////
////////////////////////////////////////////////////SETUP//
void setup() {
  pinMode(motionSensor, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600);
}
//////////////////////////////////////////////////////LOOP//
void loop() {

  int motionDetected = digitalRead(motionSensor);
  int lightSensorValue = analogRead(lightPin);
  unsigned long currentMillis = millis();

  if (motionDetected == HIGH) {
    // Motion detected, turn on LED
    digitalWrite(LED, HIGH);

    // Check light sensor to decide whether to turn on the relay
    if (lightSensorValue < lightThreshold) {
      digitalWrite(relayPin, HIGH);
      relayStatus = 1;
    } else {
      digitalWrite(relayPin, LOW);
      relayStatus = 0;
    }

    lastMillis = currentMillis;  // Reset the timer
  } else if (currentMillis - lastMillis > onTime) {
    // No motion detected for onTime duration, turn off LED and relay
    digitalWrite(LED, LOW);
    digitalWrite(relayPin, LOW);
    relayStatus = 0;
  }

  // Calculate the remaining time in seconds
  unsigned long timeRemaining;
  if (relayStatus == 1) {
    timeRemaining = (onTime - (currentMillis - lastMillis)) / 1000;
  } else {
    timeRemaining = 0;
  }

  Serial.print(" ////// MORE ON PERAN.DEV ////// ");
  Serial.print(" MOTION SENSOR (1=on, 0=off): ");
  Serial.print(motionDetected);
  Serial.print(" LIGHT SENSOR: ");
  Serial.print(lightSensorValue);
  Serial.print(" RELAY: ");
  Serial.print(relayStatus);
  Serial.print("   TIMER : ");
  Serial.print(timeRemaining);
  Serial.println(" seconds //// ");
  /////////////////////////////////////////////////END OF LOOP//
}