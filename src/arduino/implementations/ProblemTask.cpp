#include <Arduino.h>
#include "../headers/ProblemTask.h"
#include "../headers/HardwareManager.h"

ProblemTask::ProblemTask() : currentState(WORKING), stateStartTime(0) {}

void ProblemTask::init(HardwareManager* hw, SerialManager* sm) {
    hardware = hw;
    serial = sm;
}

void ProblemTask::tick() {
    Serial.println("task problem");
    switch (currentState) {
        case WORKING:
            working();
            break;

        case PROBLEM:
            problem();
            break;
    }
}

void ProblemTask::working(){
    Serial.println("Nessun problema, ancora per poco...");
    if (hardware->getTemperature() > TEMP_MAX) {
        Serial.println("Temperatura troppo alta! In attesa di restore...");
        currentState=PROBLEM;
        return;
    }
}

void ProblemTask::problem() {
    if (serial->receive()=="RESTORE") { 
        Serial.println("Comando restore ricevuto. Ripristino in corso...");
        hardware->setGreenLED(true);
        hardware->setRedLED(false);
        currentState = WORKING;
        return; 
    }
    //mettere qualche delay?
}


