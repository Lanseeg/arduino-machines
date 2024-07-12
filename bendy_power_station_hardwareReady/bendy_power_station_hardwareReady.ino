const int LED = 6;
const int motionSensor = 2;
const int relayPin = 8;
const int lightPin = A0; // Broche du capteur de luminosité
const int lightThresholdOn = 10; // Seuil de luminosité pour allumer le relais (à ajuster)
const int lightThresholdOff = 250; // Seuil de luminosité pour éteindre le relais (à ajuster)

const unsigned long onTime = 6000; // Durée en millisecondes (50 secondes)
const unsigned long debounceTime = 4000; // Temporisation en millisecondes (5 secondes)

unsigned long lastMotionDetectedMillis = 0;
unsigned long lastRelayChangeMillis = 0; // Temps du dernier changement d'état du relais
bool relayState = LOW; // État actuel du relais

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

  // Détecter le mouvement
  if (motionDetected == HIGH) {
    lastMotionDetectedMillis = currentMillis;
    digitalWrite(LED, HIGH); // Allumer la LED lorsque le mouvement est détecté
  } else {
    digitalWrite(LED, LOW); // Éteindre la LED lorsqu'aucun mouvement n'est détecté
  }

  // Décompte du temps restant pour le relais
  if (currentMillis - lastMotionDetectedMillis < onTime) {
    timeRemaining = (onTime - (currentMillis - lastMotionDetectedMillis)) / 1000;
  } else {
    timeRemaining = 0;
  }

  // Contrôle du relais basé sur la luminosité et la temporisation
  if (currentMillis - lastRelayChangeMillis > debounceTime) {
    if (lightSensorValue < lightThresholdOn && (currentMillis - lastMotionDetectedMillis < onTime)) {
      relayState = HIGH; // Allumer le relais si la luminosité est faible et le temps n'est pas écoulé
    } else if (lightSensorValue > lightThresholdOff || (currentMillis - lastMotionDetectedMillis >= onTime)) {
      relayState = LOW; // Éteindre le relais si la luminosité est élevée ou le temps est écoulé
    }
    lastRelayChangeMillis = currentMillis;
  }

  // Mettre à jour la broche du relais
  digitalWrite(relayPin, relayState);

  // Rafraîchir le moniteur série en continu
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
