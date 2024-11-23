#include "../headers/SerialManager.h"

String lastCommandForOperation = "";
String lastCommandForProblem = "";

void SerialManager::init() {
    Serial.begin(9600);
}

void SerialManager::handleCommand(String command) {
    Serial.println("Received command: " + command); // Log comando ricevuto

    if (command == "EMPTY") {
        Serial.println("Action: EMPTY command executed."); // Feedback per EMPTY
    } else if (command == "RESTORE") {
        Serial.println("Action: RESTORE command executed."); // Feedback per RESTORE
    } else {
        Serial.println("Error: Unknown command."); // Comando sconosciuto
    }
}

String SerialManager::getCommandForOperationTask() {
    String command = lastCommandForOperation;
    lastCommandForOperation = "";
    return command;
}

String SerialManager::getCommandForProblemTask() {
    String command = lastCommandForProblem;
    lastCommandForProblem = "";
    return command;
}


void SerialManager::sendTemperature(float temperature) {
    Serial.println("Temperature:" + String(temperature, 1));
}

void SerialManager::sendLevelOfWaste(int levelOfWaste) {
    Serial.println("LevelOfWaste:" + String(levelOfWaste));
}
