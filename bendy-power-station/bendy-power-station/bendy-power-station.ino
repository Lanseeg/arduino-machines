const int LED = 6;
const int motionSensor = 2;
const int relayPin = 8;

unsigned long previousTime = 0;
int lightDuration;
int lightDurationMs;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(motionSensor, INPUT);
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);                  //RELAY PIN
}

void loop() {
  int sensorValue = digitalRead(motionSensor);
  
  if (sensorValue == HIGH) {
    digitalWrite(LED, HIGH);
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(LED, LOW);
    digitalWrite(relayPin, LOW);
  }

  Serial.print("Sensor & relay activation (1=on, 0=off): ");
  Serial.print(sensorValue);
  Serial.println(" .");

}