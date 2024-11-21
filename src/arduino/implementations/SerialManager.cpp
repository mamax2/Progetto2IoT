#include "../headers/SerialManager.h"

void SerialManager::init() {
    Serial.begin(9600);
    Serial.println("Serial interface ready.");
}

void SerialManager::update() {
    if (Serial.available() != 0) {
        String command = Serial.readStringUntil('\n');
        if (command == "EMPTY") {
            // Qui cambia lo stato, in FSM avvengono le operazioni relative allo stato
            Serial.println("Emptying container...");
            delay(3000); // mettere costanti per i timer
            Serial.println("Container emptied.");
           
        } else if (command == "RESTORE") {
            Serial.println("Restoring system...");
            // anche qui cambio lo stato, in FSM faccio queste operazioni:
        }
    }
}
