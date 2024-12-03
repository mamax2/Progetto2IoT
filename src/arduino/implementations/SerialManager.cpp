#include "../headers/SerialManager.h"

String lastCommandForOperation = "";
String lastCommandForProblem = "";

/**
 * @brief Inizializzazione serial manager.
 */
void SerialManager::init() {
    Serial.begin(9600);
}

/**
 * @brief Metodo per la gestione dei comandi ricevuti.
 * @param command: Contiene il comando ricevuto.
 */
void SerialManager::handleCommand(String command) {
    Serial.println("Received command: " + command); // Log comando ricevuto

    if (command == "EMPTY") {
        Serial.println("Action: EMPTY command executed."); // Feedback per EMPTY
        lastCommandForOperation = command;
    } else if (command == "RESTORE") {
        Serial.println("Action: RESTORE command executed."); // Feedback per RESTORE
        lastCommandForProblem = command;
    } else {
        Serial.println("Error: Unknown command."); // Comando sconosciuto
    }
}

/**
 * @brief Metodo per permettere alla task operation di prendere il comando "EMPTY".
 * @return String: Contiene il comando.
 */
String SerialManager::getCommandForOperationTask() {
    String command = lastCommandForOperation;
    lastCommandForOperation = "";
    return command;
}

/**
 * @brief Metodo per permettere alla task problem di prendere il comando "PROBLEM".
 * @return String: Contiene il comando.
 */
String SerialManager::getCommandForProblemTask() {
    String command = lastCommandForProblem;
    lastCommandForProblem = "";
    return command;
}

/**
 * @brief Metodo per permettere alla task operation l'invio della temperatura.
 * @param temperature: Contiene la temperatura.
 */
void SerialManager::sendTemperature(float temperature) {
    Serial.println("Temperature:" + String(temperature, 1));
}

/**
 * @brief Metodo per permettere alla task operation l'invio del livello di riempimento.
 * @param levelOfWaste: Contiene il livello di riempimento del contenitore.
 */
void SerialManager::sendLevelOfWaste(int levelOfWaste) {
    Serial.println("LevelOfWaste:" + String(levelOfWaste));
}
