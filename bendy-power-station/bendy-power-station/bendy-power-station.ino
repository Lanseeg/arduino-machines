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
int relayStatus = 0;
///////////////////////////////////////////TIMER SETTINGS//
unsigned long lastMillis = 0;
const unsigned long onTime = 50000;
/////////////////////////////////////////TIMER SETTINGS////
////////////////////////////////////////////////////SETUP//
void setup() {
  pinMode(motionSensor, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(relayPin, OUTPUT);
  const int lightPin = A2;
  int lightSensorValue = 0;
  Serial.begin(9600);
}
//////////////////////////////////////////////////////LOOP//
void loop() {

  int motionDetected = digitalRead(motionSensor);

  Serial.print(" ////// MORE ON PERAN.DEV ////// ");
  Serial.print(" SENSOR (1=on, 0=off): ");
  Serial.print(motionDetected);
  Serial.print("   RELAY STATUS: ");
  Serial.print(relayStatus);
  Serial.println("    ////// ");

  if (motionDetected == HIGH) {
    // Motion detected, turn on LED and relay
    digitalWrite(LED, HIGH);
    digitalWrite(relayPin, HIGH);
    relayStatus = 1;
    lastMillis = millis();  // Reset the timer
  } else if (millis() - lastMillis > onTime) {
    // No motion detected for onTime duration, turn off LED and relay
    digitalWrite(LED, LOW);
    digitalWrite(relayPin, LOW);
  }
  /////////////////////////////////////////////////END OF LOOP//
}