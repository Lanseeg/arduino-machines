//////////PERANS SYSTEMS - BENDY POWER STATION/////////////
//working object-oriented programming version
//version-orientee objet fonctionnelle
//V3
////PROJECT TO BRING LIGHT AND USB CHARGING IN A CAMPING///
////SHED. MOTION SENSOR, LIGHT SENSOR, 12V RELAY & LIGHT///
////POWERED BY A BATTERY CHARGED BY A SOLAR PANNEL.     ///
/////////////////////////PERAN.DEV/////////////////////////
///////////////////////////////////////////////////////////
/////////////////////////////////////////HARDWARE ROUTING//
class MotionSensor {
public:
  MotionSensor(int pin) : pin(pin) {
    pinMode(pin, INPUT);
  }
  
  bool isMotionDetected() {
    return digitalRead(pin) == HIGH;
  }

private:
  int pin;
};

class Relay {
public:
  Relay(int pin, int lightPin, int lightThreshold) 
    : pin(pin), lightPin(lightPin), lightThreshold(lightThreshold) {
    pinMode(pin, OUTPUT);
    pinMode(lightPin, INPUT);
  }
  
  void update(unsigned long currentMillis, unsigned long lastMillis, unsigned long onTime) {
    int lightSensorValue = analogRead(lightPin);
    if (currentMillis - lastMillis < onTime && lightSensorValue < lightThreshold) {
      digitalWrite(pin, HIGH);
    } else {
      digitalWrite(pin, LOW);
    }
  }

private:
  int pin;
  int lightPin;
  int lightThreshold;
};

class LED {
public:
  LED(int pin) : pin(pin) {
    pinMode(pin, OUTPUT);
  }
  
  void on() {
    digitalWrite(pin, HIGH);
  }
  
  void off() {
    digitalWrite(pin, LOW);
  }

private:
  int pin;
};

const int motionSensorPin = 2;
const int relayPin = 8;
const int ledPin = 6;
const int lightPin = A0;
const int lightThreshold = 300;

const unsigned long onTime = 5000; // 50 seconds

MotionSensor motionSensor(motionSensorPin);
Relay relay(relayPin, lightPin, lightThreshold);
LED led(ledPin);

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (motionSensor.isMotionDetected()) {
    led.on();
    lastMillis = currentMillis; // Reset the timer
  } else {
    led.off();
  }

  relay.update(currentMillis, lastMillis, onTime);

  // Calculate the remaining time in seconds
  unsigned long timeRemaining = 0;
  if (digitalRead(relayPin) == HIGH) {
    timeRemaining = (onTime - (currentMillis - lastMillis)) / 1000;
  }

  // Refresh Serial Monitor continuously
  Serial.print("Motion Detected: ");
  Serial.print(motionSensor.isMotionDetected());
  Serial.print(", Light Sensor Value: ");
  Serial.print(analogRead(lightPin));
  Serial.print(", Relay Status: ");
  Serial.print(digitalRead(relayPin));
  Serial.print(", Time Remaining: ");
  Serial.print(timeRemaining);
  Serial.println(" seconds");
}
