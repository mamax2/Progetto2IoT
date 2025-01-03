#include "../headers/HardwareManager.h"

HardwareManager::HardwareManager() : lcd(RS_PIN, EN, D4, D5, D6, D7) {}

void HardwareManager::init() {
    pinMode(PIR_PIN, INPUT);
    pinMode(BUTTON_OPEN, INPUT);
    pinMode(BUTTON_CLOSE, INPUT);
    pinMode(TEMP_SENSOR, INPUT);
    pinMode(TRIG_WASTE_SENSOR, OUTPUT); // Sets the trigPin as an Output
    pinMode(ECHO_WASTE_SENSOR, INPUT); // Sets the echoPin as an Input
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);

    doorServo.attach(3);
    moveMotor(90);

    //initializing lcd
    lcd.begin(16,2);
}

void HardwareManager::update() {
    userDetected = digitalRead(PIR_PIN);
    int rawValue = analogRead(TEMP_SENSOR); // Leggi il valore analogico (0-1023)
    float voltage = rawValue * (5.0 / 1023.0); // Converti in tensione (0-5V)
    temperature = (voltage-0.5) * 100.0; // 10 mV per grado Celsius (LM35)
    digitalWrite(TRIG_WASTE_SENSOR, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIG_WASTE_SENSOR, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_WASTE_SENSOR, LOW);
    wasteLevel = pulseIn(ECHO_WASTE_SENSOR, HIGH) * 0.034 / 2;
}

bool HardwareManager::isUserDetected() {
    return userDetected;
}

float HardwareManager::getTemperature() {
    return temperature;
}

int HardwareManager::getWasteLevel() {
    return wasteLevel;
}

void HardwareManager::setGreenLED(bool state) {
    digitalWrite(GREEN_LED, state);
}

void HardwareManager::setRedLED(bool state) {
    digitalWrite(RED_LED, state);
}

void HardwareManager::openDoor() {
    moveMotor(180);
}

void HardwareManager::closeDoor() {
    moveMotor(90);
}

void HardwareManager::reverseDoor() {
    moveMotor(0);
}

void HardwareManager::moveMotor(int pos){
    if (pos > 180){
		pos = 180;
	} else if (pos < 0){
		pos = 0;
	}
  float coeff = (2400.0-544.0)/180;
  doorServo.write(544 + pos*coeff); 
}

void HardwareManager::displayMessage(String line1, String line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}
