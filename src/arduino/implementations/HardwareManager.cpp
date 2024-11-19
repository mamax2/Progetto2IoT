#include "HardwareManager.h"

HardwareManager::HardwareManager() : lcd(0x27, 16, 2) {}

void HardwareManager::init() {
    pinMode(PIR_PIN, INPUT);
    pinMode(BUTTON_OPEN, INPUT_PULLUP);
    pinMode(BUTTON_CLOSE, INPUT_PULLUP);
    pinMode(TEMP_SENSOR, INPUT);
    pinMode(WASTE_SENSOR, INPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);

    doorServo.attach(3);
    doorServo.write(0);

    lcd.begin();
    lcd.print("Initializing...");
    delay(1000);
    lcd.clear();
}

void HardwareManager::update() {
    userDetected = digitalRead(PIR_PIN);
    temperature = analogRead(TEMP_SENSOR) * (5.0 / 1023.0) * 100;
    wasteLevel = analogRead(WASTE_SENSOR);
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
    doorServo.write(90);
}

void HardwareManager::closeDoor() {
    doorServo.write(0);
}

void HardwareManager::reverseDoor() {
    doorServo.write(-90);
}

void HardwareManager::displayMessage(const char* line1, const char* line2) {
    lcd.clear();
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}
