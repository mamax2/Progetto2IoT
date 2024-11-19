#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Componenti hardware
#define PIR_PIN 5          // Pin del rilevatore PIR
#define BUTTON_OPEN 8      // Pulsante per aprire la porta
#define BUTTON_CLOSE 10     // Pulsante per chiudere la porta
#define TEMP_SENSOR A2     // Sensore di temperatura
#define GREEN_LED 12        // LED verde
#define RED_LED 11          // LED rosso
#define WASTE_SENSOR 7    // Sensore di livello del contenitore
#define SERVO   3

// Misure
#define DISTANCE_THRESHOLD //DA DECIDERE // Soglia del sensore di livello
#define MAX_TEMP //DA DECIDERE        // Temperatura massima consentita
#define MAX_TEMP_TIME 5000 // Tempo massimo oltre la soglia di temperatura

// Display LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo motore per la porta
Servo doorServo;

// Variabili per la logica
enum State {
  IDLE, WAIT_OPEN, OPEN, WAIT_CLOSE, FULL, PROBLEM
};
State currentState = IDLE;

unsigned long lastTempCheckTime = 0;
unsigned long tempOverTime = 0;
unsigned long openTimeout = 0;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_OPEN, INPUT);
  pinMode(BUTTON_CLOSE, INPUT);
  pinMode(TEMP_SENSOR, INPUT);
  pinMode(WASTE_SENSOR, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Inizializza servo e LCD
  doorServo.attach(SERVO);
  doorServo.write(0);
  lcd.begin();
  lcd.print("Initializing...");
  delay(1000);
  
  // Stato iniziale
  currentState = IDLE;
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  lcd.clear();
  lcd.print("PRESS OPEN TO");
  lcd.setCursor(0, 1);
  lcd.print("ENTER WASTE");
}

void loop() {
  int pirDetected = digitalRead(PIR_PIN);
  int wasteLevel = analogRead(WASTE_SENSOR);
  int tempValue = analogRead(TEMP_SENSOR);
  float temperature = tempValue * (5.0 / 1023.0) * 100;

  switch (currentState) {
    case IDLE:
      if (pirDetected) {
        lcd.clear();
        lcd.print("PRESS OPEN TO");
        lcd.setCursor(0, 1);
        lcd.print("ENTER WASTE");
        currentState = WAIT_OPEN;
      }
      break;

    case WAIT_OPEN:
      if (digitalRead(BUTTON_OPEN) == LOW) {
        doorServo.write(90);
        lcd.clear();
        lcd.print("DOOR OPENED");
        lcd.setCursor(0, 1);
        lcd.print("PRESS CLOSE");
        openTimeout = millis();
        currentState = OPEN;
      }
      break;

    case OPEN:
      if (digitalRead(BUTTON_CLOSE) == LOW || millis() - openTimeout > 10000) {
        doorServo.write(0);
        lcd.clear();
        lcd.print("WASTE RECEIVED");
        lcd.setCursor(0, 1);
        lcd.print("THANK YOU!");
        delay(2000);
        currentState = IDLE;
      }
      break;

    case FULL:
      lcd.clear();
      lcd.print("CONTAINER FULL");
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      break;

    case PROBLEM:
      lcd.clear();
      lcd.print("PROBLEM DETECTED");
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      break;
  }

  // Controllo temperatura
  if (temperature > MAX_TEMP) {
    if (millis() - lastTempCheckTime > MAX_TEMP_TIME) {
      currentState = PROBLEM;
    }
  } else {
    lastTempCheckTime = millis();
  }

  // Controllo livello contenitore
  if (wasteLevel < DISTANCE_THRESHOLD && currentState != FULL && currentState != PROBLEM) {
    currentState = FULL;
  }
}
