#include <Arduino.h>
#include "../headers/ProblemTask.h"
#include "../headers/HardwareManager.h"

ProblemTask::ProblemTask(HardwareManager* hw, SerialManager* sm, OperationTask* ot) : currentState(WORKING), stateStartTime(0) {
    hardware = hw;
    serial = sm;
    operationtask=ot;
    init();
}

void ProblemTask::init() {}

void ProblemTask::tick() {

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
    if (hardware->getTemperature() > TEMP_MAX && operationtask->getProblemFlag()==false) {
        operationtask->setProblemFlag(true);
        hardware->setGreenLED(false);
        hardware->setRedLED(true);
        hardware->closeDoor();
        hardware->displayMessage("PROBLEM","DETECTED");
        currentState=PROBLEM;
        return;
    }
}

void ProblemTask::problem() {
    serial->handleCommand(Serial.readStringUntil('\n'));
    if (serial->getCommandForProblemTask() == "RESTORE") {
        currentState = WORKING;
        hardware->setGreenLED(LOW);
        hardware->setRedLED(HIGH);
        operationtask->setProblemFlag(false);
        stateStartTime = millis();
        return;
    }
}