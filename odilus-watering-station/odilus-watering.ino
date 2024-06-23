//////////PERANS SYSTEMS WATERING STATION - ODILUS/////////
/////////////////////////PERAN.DEV/////////////////////////
//////////////MOISTURE & TIME SETTINGS/////////////////////
//MOISTURE - HUMIDITE                    ////A5 - MOISTURE/
float moistSet = 70; //WAS CONST FLOAT///////////////////
const int PinMoisture = A5;                            //
const int Dry = 596;  //CALIBRATION
const int Wet = 251;  //CALIBRATION
int soilRawValue = 0;
int soilMoisture = 0;                                  //

//TIME MANAGEMENT//                         //////TIME/////
unsigned long previousTime = 0;
                                            ////INTERVAL///
long pumpInterval;
long pumpIntervalMs;
                                           //PUMP DURATION//
int pumpDuration;
int pumpDurationMs;

//TEMPERATURE//                             //A0 - TEMP///
const int temPin = A0;
int TmpC; //WAS FLOAT DATATYPE
//LIGHT SENSOR//                            ///A2 -LIGHT//
const int lightPin = A2;
int lightSensorValue = 0;
//RELAY//                                   //D8 - RELAY//
const int motorPin = 8;
//TEST BUTTON WAS
//const int testButton = 3;
int testStatus = 0;                  //D13 - STATUS LED///
//STATUS LED
const int LEDpin = 13;
const int LEDrun = 7;             //D7 - PUMP STATUS LED//
const long onDuration = 5;
const long offDuration = 4500;
int LEDState = HIGH;
long blinkTime = 0; //previousTime
//////////////////////////////10 - FLOTTEUR FLOAT SWITCH//
const int floatSwitch = 10; //WAS ANALOGIC 3 (10)///////
const unsigned long safeTime = 1500;                  //
int floatIcon;
//RED LED///////////////////////////////////////////////
const int emptyLED = 6;  //D11 - WATER TANK EMPTY LED //
int floatState = 0;                                   //
//BEGIN ANALOGIC BUTTONS 0 1 2///A1 - 3 ANALOGIC BUTTONS//
//Source code on https://modelleisenbahn.triskell.org/spip.php?article59/ //
//Plusieurs boutons poussoir sur une entrée analogique  //
const byte NON_PRESSE = 0;                              //
const byte ENFONCE = 1;
const byte PRESSE = 2;
//
byte etatAutomate = NON_PRESSE;
int etatPoussoir = -1;
//
const byte AUCUN_EVENEMENT = 0;
const byte EVENEMENT_PRESSE = 1;
const byte EVENEMENT_RELACHE = 2;
//
const int pinPoussoirs = A1;
//
int numPoussoir;
int lirePoussoirs()
{
  int resultat;
  int numPoussoir = (analogRead(pinPoussoirs) + 170) / 341;

  int nouvelEtatPoussoir = etatPoussoir;

  switch (etatAutomate) {         //SWITCH EXAMPLE//////
    case NON_PRESSE:
      if (numPoussoir < 3)
        etatAutomate = ENFONCE;
      break;
    case ENFONCE:
      if (numPoussoir < 3) {
        etatAutomate = PRESSE;
        nouvelEtatPoussoir = numPoussoir;
      }
      else {
        etatAutomate = NON_PRESSE;
      }
      break;
    case PRESSE:
      if (numPoussoir == 3) {
        etatAutomate = NON_PRESSE;
        nouvelEtatPoussoir = -1;
      }
      break;
  }
  return nouvelEtatPoussoir;
}                                                     //
/*
   construction d'un événement en comparant
   le nouvel état des poussoirs avec l'état précédent.
*/
byte lireEvenement(int *numPoussoir)
{
  byte evenement;
  int nouvelEtatPoussoir = lirePoussoirs();

  if (nouvelEtatPoussoir == etatPoussoir)
    evenement = AUCUN_EVENEMENT;
  if (nouvelEtatPoussoir >= 0 && etatPoussoir == -1)
    evenement = EVENEMENT_PRESSE;
  if (nouvelEtatPoussoir == -1 && etatPoussoir >= 0)
    evenement = EVENEMENT_RELACHE;

  etatPoussoir = nouvelEtatPoussoir;
  *numPoussoir = etatPoussoir;
  return evenement;
}
//END OF A1 - ANALOGIC BUTTONS 0 1 2 setup /////////////
//////////////////////   L   C   D   ///////////////////
//LCD BACKLIGHT - A3 ///////////////SCREENSAVER//SETUP//
const int backLight = A3;              /////////////////
const byte BACKLIGHT_INTENSITY = 255;  /////////////////
const unsigned long onTime = 120000;   /////////////////
//END OF LCD BACKLIGHT//////////////////////////////////
//BEGIN OF LCD//////////////////////////////////////////
//LCD RS pin to digital pin 12
//LCD Enable pin to digital pin 11
//LCD D4 pin to digital pin 5
//LCD D5 pin to digital pin 4
//LCD D6 pin to digital pin 3
//LCD D7 pin to digital pin 2
//LCD R/W pin to ground
//LCD VSS pin to ground
//LCD VCC pin to 5V//OR SHUT DOWN LCD USING DIGITAL PIN//
//10K resistor:
//ends to +5V and ground
//wiper to LCD VO pin (pin 3)
// http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
////////////////////////SPECIAL CHARACTERS////////////////
byte logo[] = {
  B11100,
  B11100,
  B11100,
  B10000,
  B10000,
  B00000,
  B10101,
  B01010
};
byte raindrop[] = {
  B00100,
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100
};
byte thermoIcon[] = {
  B01110,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};
byte motorIcon[] = {
  B00000,
  B10011,
  B11010,
  B00100,
  B01011,
  B11001,
  B00000,
  B00000
};
byte intervalIcon[] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};
byte emptytankIcon[] = {
  B11011,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};
byte fulltankIcon[] = {
  B11011,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
////////////////////////////////////
//END OF LCD////////////////////////
//MENU/////////////////////////////////////////MENU////////////////
//adapted from https://myhomethings.eu/en/dynamic-arduino-lcd-menu/
//const byte upButtonPin = ?;
const byte upButtonPin(numPoussoir == 0);       //LEFT +          //
//const byte downButtonPin = ?;
//const byte rightButtonPin = A1;
const byte rightButtonPin(numPoussoir == 1);    //MIDDLE -        //
//const byte leftButtonPin = ?;
const byte leftButtonPin(numPoussoir == 2);     //RIGHT - SELECT  //
//
const byte tweeterPin = 9;                      //BEEP SOUND PIN  //

String MenuItems[] = {  // Your menu items
  "PWS Odilus  Menu",
  "Moisture settings",
  "Pump duration",
  "Interval",
  "Run Pump Test",
};
//MENU////END///////////////////////

void setup() {                                                      //VOID SETUP////////                  ////SETUP////

  //////////////////////                      //PUMP DURATION & INTERVAL//
  pumpDuration = 8;
  pumpInterval = 5;
  //////////////////////INITAL VALUES///////////

  Serial.begin(9600);                         //
  pinMode(motorPin, OUTPUT);                  //RELAY PIN
  pinMode(temPin, INPUT);                     //TEMP SENSOR
  pinMode(LEDpin, OUTPUT);                    //STATUS LED
  digitalWrite(LEDpin, LEDState);             //
  pinMode(LEDrun, OUTPUT);                    //PUMP LED
  //pinMode(testButton,INPUT); //for older versions (V4 or V5)
  ////////FLOATSWITCH SETUP////////////       //FLOATSWITCH
  pinMode(floatSwitch, INPUT_PULLUP);         //
  pinMode(emptyLED, OUTPUT);                  //EMPTY LED
  /////////////////END OF FLOATSWITCH SETUP/////
  pinMode(upButtonPin, INPUT_PULLUP);         //NAVIGATION BUTTONS
  //pinMode(downButtonPin, INPUT_PULLUP);     //
  pinMode(rightButtonPin, INPUT_PULLUP);      //
  pinMode(leftButtonPin, INPUT_PULLUP);       //
  pinMode(pinPoussoirs, INPUT);               //PIN POUSSOIR AS AN INPUT//
  lcd.begin(16, 2);                           //LCD BEGIN
  lcd.clear();                                //
  //LCD BACKLIGHT///////////////////////////////LCD BACKLIGHT
  pinMode (backLight, OUTPUT);                //
  analogWrite(backLight, BACKLIGHT_INTENSITY);//
  //END OF LCD BACKLIGHT////////////////////////
  //////////CUSTOMCHAR//////////////////////////
  //lcd.home();
  lcd.createChar(1, logo);
  lcd.createChar(2, raindrop);
  lcd.createChar(3, thermoIcon);
  lcd.createChar(4, motorIcon);
  lcd.createChar(5, intervalIcon);
  lcd.createChar(6, emptytankIcon);
  lcd.createChar(7, fulltankIcon);
  //END OF CUSTOM CHAR//                           //
  //LCD STARTUP MESSAGE
  lcd.setCursor(0, 0);                             //
  lcd.write(1);                                    //
  lcd.setCursor(2, 0);                             //
  lcd.print("PERANS SYSTEMS");                     //LCD STARTUP MESSAGE
  lcd.setCursor(0, 1);
  lcd.print("Watering Station");                   //LCD STARTUP MESSAGE
  delay(3500);
  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.write(1);
  digitalWrite(LEDpin, HIGH);
  delay(100);
  lcd.setCursor(7, 1);
  lcd.write(2);
  digitalWrite(LEDrun, HIGH);
  delay(150);
  lcd.setCursor(8, 0);
  lcd.write(2);
  delay(180);
  lcd.setCursor(8, 1);
  lcd.write(1);
  digitalWrite(emptyLED, HIGH);
  delay(800);
  lcd.clear();
  digitalWrite(LEDpin, LOW);
  digitalWrite(LEDrun, LOW);
  digitalWrite(emptyLED, LOW);
  beepsOnce();                                     //STARTUP BEEP
  delay(50);
  beepsOnce();
}                                                  //                         //END OF SETUP//

//////MENU//FUNCTIONS//////////////                 //MENU FUNCTIONS//////
void menuFunctions(int menu, byte right, byte left)     //
{
  static unsigned long lastUpdat = millis();//?
  if (menu == 1) //ACCUEIL                             //MENU 1
  {
    lcd.setCursor(0, 1);
    lcd.print("LIST  UPDATE");
    lcd.setCursor(15, 1);
    lcd.write(5);

    if (right == 1)
    {
      beepsOnce();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("LUMINOSITY SENSOR");
      lcd.setCursor(0, 1);
      lcd.print("Light :        ");
      lcd.setCursor(13, 1);
      lcd.print(lightSensorValue);
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SOIL SENSOR");
      lcd.setCursor(0, 1);
      lcd.print("Moisture ");
      lcd.setCursor(10, 1);
      lcd.write(2);
      lcd.setCursor(11, 1);
      lcd.print(soilMoisture);
      lcd.setCursor(14, 1);
      lcd.print("%");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PWS Odilus  Menu");
      lcd.setCursor(0, 1);
      if (floatIcon == 0) {
        lcd.write(6);
        lcd.setCursor(1, 1);
        lcd.print("NO WATER");
        lcd.setCursor(9, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.print(TmpC);
        lcd.setCursor(13, 1);
        lcd.print("deg");
      }
      if (floatIcon == 1) {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.write(7);
        lcd.setCursor(0, 0);
        lcd.print("PWS Odilus  Menu");
        lcd.setCursor(2, 1);
        lcd.write(2);
        lcd.setCursor(3, 1);
        lcd.print(soilMoisture);//remplacer par l'humidité relevée
        lcd.setCursor(5, 1);
        lcd.print("%");
        lcd.setCursor(9, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.print(TmpC);
        lcd.setCursor(13, 1);
        lcd.print("deg");
      }
    }
    if (left == 1)
    {
      beepsOnce();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PWS Odilus  Menu");
      lcd.setCursor(5, 1);
      lcd.write(1);
      delay(120);
      lcd.setCursor(6, 1);
      lcd.write(2);
      delay(140);
      lcd.setCursor(7, 1);
      lcd.write(3);
      delay(160);
      lcd.setCursor(8, 1);
      lcd.write(1);
      delay(400);
      lcd.setCursor(0, 1);
      if (floatIcon == 0) {
        lcd.write(6);
        lcd.setCursor(1, 1);
        lcd.print("NO WATER");
        lcd.setCursor(9, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.print(TmpC);
        lcd.setCursor(13, 1);
        lcd.print("deg");
      }
      if (floatIcon == 1) {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.write(7);
        lcd.setCursor(0, 0);
        lcd.print("PWS Odilus  Menu");
        lcd.setCursor(2, 1);
        lcd.write(2);
        lcd.setCursor(3, 1);
        lcd.print(soilMoisture);//remplacer par l'humidité relevée
        lcd.setCursor(5, 1);
        lcd.print("%");
        lcd.setCursor(9, 1);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.print(TmpC);
        lcd.setCursor(13, 1);
        lcd.print("deg");
      }
    }
  }
  if (menu == 2) // Moisture settings                         //MENU 2
  {
    if (right == 1)
    {
      moistSet += 5;
      if (moistSet >= 100)
      {
        moistSet = 100;
      }
    }
    if (left == 1)
    {
      moistSet -= 5;
      if (moistSet <= 25)
      {
        moistSet = 25;
      }
    }
    lcd.setCursor(0, 1);
    lcd.write(2);
    lcd.setCursor(2, 1);
    lcd.print("set:");
    lcd.setCursor(13, 1);
    lcd.print(moistSet);
    lcd.print("%");
    //analogWrite(ledPin_fade, moistVal);
  }
  if (menu == 3) // PUMP DURATION                             //MENU 3
  {
    if (right == 1)
    {
      pumpDuration += 1;
      if (pumpDuration >= 60
         )
      {
        pumpDuration = 60;
      }
    }
    if (left == 1)
    {
      pumpDuration -= 1;
      if (pumpDuration <= 1)
      {
        pumpDuration = 1;
      }
    }
    lcd.setCursor(0, 1);
    lcd.write(4);
    lcd.setCursor(2, 1);
    lcd.print("set:");
    lcd.setCursor(11, 1);
    lcd.print(pumpDuration);
    lcd.print("sec");
  }
  if (menu == 4) // PUMP INTERVAL                             //MENU 4
  {
    if (right == 1)
    {
      pumpInterval += 1;
      if (pumpInterval >= 120)
      {
        pumpInterval = 120;
      }
    }
    if (left == 1)
    {
      pumpInterval -= 1;
      if (pumpInterval <= 1)
      {
        pumpInterval = 1;
      }
    }
    lcd.setCursor(0, 1);
    lcd.write(5);
    lcd.setCursor(2, 1);
    lcd.print("set:");
    lcd.setCursor(10, 1);
    lcd.print(pumpInterval);
    lcd.print("min");
  }
  if (menu == 5) // PUMP TEST                               //MENU 5
  {
    lcd.setCursor(0, 1);
    lcd.print("<RUN  STOP MENU>");
    if (right == 1)
    {
      lcd.setCursor(0, 1);
      lcd.write(4);
      lcd.setCursor(1, 1);
      lcd.print("RUNNING X  MENU>");
      beepsOnce();
      digitalWrite(motorPin, HIGH);
      digitalWrite(LEDrun, HIGH);
      lcd.noDisplay();
      delay(500);
      // Turn on the display:
      lcd.display();
      delay(500);
      beepsOnce();
      //digitalWrite(ledPin, HIGH);//REMPLACER PAR ACTIVER LE RELAIS
    }
    if (left == 1)
    {
      lcd.setCursor(0, 1);
      lcd.print("<RUN  STOP MENU>");
      digitalWrite(motorPin, LOW);
      digitalWrite(LEDrun, LOW);
      //digitalWrite(ledPin, LOW);//REMPLACER PAR STOP POMPE
    }
  }
}
////MENU & BUZZER AUTOMATION////
/***  do not modify  ***********************************************/
template< typename T, size_t NumberOfSize >
size_t MenuItemsSize(T (&) [NumberOfSize]) {
  return NumberOfSize;
}
int numberOfMenuItems = MenuItemsSize(MenuItems) - 1;
int currentMenuItem = 0;
int previousMenuItem = 1;
byte button_flag = 0;
unsigned long previousMillis = millis();
const int note = 4699;
void beepsOnce()                  //BEEP SETTINGS
{
  tone(tweeterPin, note, 125);
  delay(60);
  noTone(tweeterPin);
}
/*************END******OF*MENU*****FONCTIONS************************/
/////////////////////////////////////////////////////////////////////
void loop() {                                         //////L O O P//                                                     ///VOID LOOP//

  //PUMP DURATION & INTERVAL//VALUES UPDATE//
  pumpDurationMs = pumpDuration * 1000;
  pumpIntervalMs = (pumpInterval * 60000);
  //////////////////////////////////////////

  ////////////////////////LCD SCREENSAVER LOOP////////////SCREENSAVER//
  //TOPIC on https://forum.arduino.cc/t/compte-a-rebours-timer-lcd/880340/6//
  ///////////////////////////////////////POWERS ON & OFF THE LCD BACKLIGHT//
  static unsigned long lastMillis = millis();
  int poussVal = analogRead(pinPoussoirs);
  //Serial.print("Val: ");
  //Serial.println(poussVal); //lecture du Pin bouton poussoir
  //delay(50);
  //
  if (millis() - lastMillis >= onTime)
  {
    analogWrite(backLight, 0);
  }
  if (poussVal <= 1000) {
    lastMillis = millis();
    analogWrite(backLight, BACKLIGHT_INTENSITY);
  }                                                                 //
  /////////////////////END OF LCD SCREENSAVER///////////////////////////
  //BEGIN LOOP ANALOGIC BUTTONS 0 1 2 setup ///////ANALOGIG BUTTONS///

  int numPoussoir;
  byte evenement = lireEvenement(&numPoussoir);

  switch (evenement) {
    case EVENEMENT_PRESSE:
      Serial.print("Presse : ");
      Serial.println(numPoussoir);
      break;
    case EVENEMENT_RELACHE:
      Serial.print("Relache : ");
      Serial.println(numPoussoir);
      break;
  }
  delay(3);
  ///////////////////////////END LOOP ANALOGIC BUTTONS 0 1 2 setup ////
  ////////////////////////MENU//////////AUTOMATION/////////////////////
  //if(digitalRead(rightButtonPin) == LOW && button_flag == 0)
  if (numPoussoir == 0 && button_flag == 0)
  {
    menuFunctions(currentMenuItem + 1, 1, 0);
    button_flag = 1;
    previousMillis = millis();
    beepsOnce();
  }
  //if(digitalRead(leftButtonPin) == LOW && button_flag == 0)
  if (numPoussoir == 1 && button_flag == 0)
  {
    menuFunctions(currentMenuItem + 1, 0, 1);
    button_flag = 1;
    previousMillis = millis();
    beepsOnce();
  }
  //if(digitalRead(upButtonPin) == LOW && button_flag == 0)
  if (numPoussoir == 2 && button_flag == 0)
  {
    ++currentMenuItem;
    if (currentMenuItem > numberOfMenuItems )
    {
      currentMenuItem = 0 ; //WAS numberOfMenuItems
    }
    button_flag = 1;
    previousMillis = millis();
    beepsOnce();
  }
  /*  else if(digitalRead(downButtonPin) == LOW && button_flag == 0)
    {
      currentMenuItem--;
      if(currentMenuItem < 0)
      {
        currentMenuItem = 0;
      }
      button_flag = 1;
      previousMillis = millis();
      beepsOnce();}
  */
  if (currentMenuItem != previousMenuItem)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(MenuItems [currentMenuItem]);
    menuFunctions(currentMenuItem + 1, 0, 0);
    previousMenuItem = currentMenuItem;
  }
  if (millis() - previousMillis >= 400)
  {
    previousMillis = millis();
    button_flag = 0;
  }
  //END OF MENU//////////AUTOMATION//////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  ////LED BLINKING////////////////////////////STATUS LED BLINKING////
  if ( LEDState == HIGH )
  {
    if ( (millis() - blinkTime) >= onDuration) {
      LEDState = LOW;// change the state of LED
      blinkTime = millis(); // remember Current millis() time
    }
  }
  else
  {
    if ( (millis() - blinkTime) >= offDuration) {
      LEDState = HIGH; // change the state of LED
      blinkTime = millis(); // remember Current millis() time
    }
  }
  digitalWrite(LEDpin, LEDState);
  ////END OF LED BLINKING////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  //SENSORS SERIAL DISPLAY//////////////////////////SERIAL DISPLAY///
  soilRawValue = analogRead(PinMoisture); /////////MOISTURE DISPLAY//
  Serial.print("moist: ");
  //Serial.print(soilRawValue);
  soilMoisture = map(soilRawValue, Dry, Wet, 0, 100);
  if (soilMoisture <= 0) {
    soilMoisture = 0;}
  Serial.print(" moist: ");
  Serial.print(soilMoisture);
  Serial.print(" % \t");
  lightSensorValue = analogRead(lightPin); //LUMINOSITY DISPLAY
  Serial.print("  Light.: ");
  Serial.print(lightSensorValue);
  Serial.print("\t");
  delay(5);
  int TmpVal = analogRead(temPin); //TEMPERATURE DISPLAY
  Serial.print("Temp: ");
  //Serial.print(TmpVal);
  float TmpVolt = (TmpVal / 1024.0) * 5.0;
  //Serial.print(", Volts : ");
  //Serial.print(TmpVolt);
  Serial.print(" deg ");
  TmpC = (TmpVolt - .5) * 100;//WAS FLOAT DATA
  Serial.print(TmpC);
  Serial.print(" C \t");
  delay(15); //LINE DISPLAY RATE --END OF VALUES DISPLAY--//
  //END OF SENSORS DISPLAY//

  //SETTINGS DISPLAY//
  Serial.print("SETTINGS: moist ");
  Serial.print(moistSet);
  Serial.print("  ");
  Serial.print("Interval ");
  Serial.print(pumpInterval);
  Serial.print("  ");
  Serial.print("Duration ");
  Serial.println(pumpDuration);
  //END OF SETTINGS DISPLAY//
  ////////////////////////////////////////////////END OF SERIAL DISPLAY//

  //////////////////////////////////////////FLOAT BUTTON LOOP////////////
  floatState = digitalRead(floatSwitch);
  //Serial.print("Val float: ");
  //Serial.println(floatState);
  static unsigned long floatTime = millis();
  if (floatState >= 1) {          //IF WATER TANK EMPTY
    floatTime = millis();
    //Serial.println(" NO WATER");
    digitalWrite(emptyLED, HIGH);
    floatIcon = 0;
  }
  if (millis() - floatTime >= safeTime) {   /*loop to avoid noise in float switch readings*/
    if (floatState <= 0) {        //IF WATER TANK FILLED//SENSORS CONDITIONS TO LAUNCH PUMP//
      //Serial.println("WATER TANK OK");
      digitalWrite(emptyLED, LOW);
      floatIcon = 1;
      //Serial.println(" WATER TANK OK");
    }

    //TIME GAP -- CALCUL DE L'INTERVALE DE TEMPS//INTERVAL LOOP -   BEGIN OF WATERING FUNCTIONS//
    unsigned long currentTime = millis();
    if (currentTime - previousTime >= pumpIntervalMs && floatIcon == 1) {   //TIME BETWNEEN WATERINGS
      previousTime = currentTime; //END OF TIME GAP

      //COLD WEATHER
      if (soilMoisture < moistSet && TmpC > 2 && TmpC < 7  && lightSensorValue > 25)
      {
        //temperature between 2&10 degree C, enough light to avoid freezing -->GO
        digitalWrite(motorPin, HIGH);
        digitalWrite(LEDrun, HIGH);
        Serial.println("PUMP RUNNING COLD WEATHER SETTING");
        delay(pumpDurationMs); //TEMPS FONCTIONNEMENT POMPE - TIME THE PUMP WILL RUN
        digitalWrite(motorPin, LOW);
        Serial.println("PUMP STOPPING, PAUSE...");
        delay(1000); //PAUSE TIME SETUP
        Serial.println("CHECKING ...");
        digitalWrite(LEDrun, LOW);
        delay(500);
      }
      else {
        digitalWrite(motorPin, LOW);
      }// END OF COLD WEATHER

      //REGULAR WEATHER
      if (soilMoisture < moistSet && TmpC > 7 && TmpC < 29)
      {
        //temperature between 10&27 degree C, no matter the daytime or night time -->GO
        digitalWrite(motorPin, HIGH);
        digitalWrite(LEDrun, HIGH);
        Serial.println("PUMP RUNNING REGULAR WEATHER SETTING");
        delay(pumpDurationMs); //TEMPS FONCTIONNEMENT POMPE - TIME THE PUMP WILL RUN
        digitalWrite(motorPin, LOW);
        Serial.println("PUMP STOPPING, PAUSE...");
        delay(1000); //PAUSE TIME SETUP
        Serial.println("CHECKING ...");
        digitalWrite(LEDrun, LOW);
        delay(500);
      }
      else {
        digitalWrite(motorPin, LOW);
      }// END OF REGULAR WEATHER

      //HOT WEATHER
      if (soilMoisture < moistSet && TmpC > 29 && lightSensorValue < 300)
      {
        //temperature hotter than 27 degree C, during night time -->GO
        digitalWrite(motorPin, HIGH);
        digitalWrite(LEDrun, HIGH);
        Serial.println("PUMP RUNNING HOT WEATHER SETTING");
        delay(pumpDurationMs); //TEMPS FONCTIONNEMENT POMPE - TIME THE PUMP WILL RUN
        digitalWrite(motorPin, LOW);
        Serial.println("PUMP STOPPING, PAUSE...");
        delay(1000); //PAUSE TIME SETUP
        Serial.println("CHECKING ...");
        digitalWrite(LEDrun, LOW);
        delay(500);
      }
      else {
        digitalWrite(motorPin, LOW);
      }// END OF HOT WEATHER

      delay(10);
    }//FIN FONCTION INTERVAL (MILLIS UNSIGNED LONG) ///////////////
  }////FIN FONCTION WATER TANK SWITCH//////////////////////////////
}//END OF LOOP////////////////////////////////////////////////2021/
