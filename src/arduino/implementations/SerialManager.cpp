#include "SerialManager.h"

void SerialManager::init() {
    Serial.begin(9600);
    Serial.println("Serial interface ready.");
}

void SerialManager::update() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        if (command == "EMPTY") {
            Serial.println("Emptying container...");
            hardware->openDoor();
            delay(3000);
            hardware->closeDoor();
            Serial.println("Container emptied.");
        } else if (command == "RESTORE") {
            Serial.println("Restoring system...");
            hardware->setGreenLED(true);
            hardware->setRedLED(false);
        }
    }
}
