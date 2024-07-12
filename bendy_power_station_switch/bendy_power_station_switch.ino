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
const int lightPin = A0; // Broche du capteur de luminosité
const int lightThreshold = 300; // Seuil de luminosité pour le jour (à ajuster selon vos besoins)

const unsigned long onTime = 5000; // Durée en millisecondes (50 secondes)

unsigned long lastMillis = 0;
enum State {IDLE, ACTIVE, COUNTDOWN};
State currentState = IDLE;

void setup() {
  pinMode(motionSensor, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(lightPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int motionDetected = digitalRead(motionSensor);
  int lightSensorValue = analogRead(lightPin);
  unsigned long currentMillis = millis();
  unsigned long timeRemaining = 0;

  switch (currentState) {
    case IDLE:
      if (motionDetected == HIGH) {
        digitalWrite(LED, HIGH); // Turn on LED when motion is detected
        if (lightSensorValue < lightThreshold) {
          digitalWrite(relayPin, HIGH);
        }
        lastMillis = currentMillis;
        currentState = ACTIVE;
      } else {
        digitalWrite(LED, LOW); // Ensure LED is off when no motion is detected
      }
      break;

    case ACTIVE:
      if (motionDetected == HIGH) {
        lastMillis = currentMillis; // Reset the timer if motion is detected
        digitalWrite(LED, HIGH); // Keep LED on while motion is detected
      } else {
        digitalWrite(LED, LOW); // Turn off LED if no motion
      }

      if (currentMillis - lastMillis > onTime) {
        digitalWrite(relayPin, LOW); // Turn off relay after onTime
        currentState = IDLE;
      } else {
        if (lightSensorValue < lightThreshold) {
          digitalWrite(relayPin, HIGH);
        } else {
          digitalWrite(relayPin, LOW);
        }
        currentState = COUNTDOWN;
      }
      break;

    case COUNTDOWN:
      timeRemaining = (onTime - (currentMillis - lastMillis)) / 1000;
      currentState = ACTIVE;
      break;
  }

  // Refresh Serial Monitor continuously
  Serial.print("Motion Detected: ");
  Serial.print(motionDetected);
  Serial.print(", Light Sensor Value: ");
  Serial.print(lightSensorValue);
  Serial.print(", Relay Status: ");
  Serial.print(digitalRead(relayPin));
  Serial.print(", Time Remaining: ");
  Serial.print(timeRemaining);
  Serial.println(" seconds");
}