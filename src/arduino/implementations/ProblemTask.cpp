#include <Arduino.h>
#include "../headers/ProblemTask.h"
#include "../headers/HardwareManager.h"

ProblemTask::ProblemTask(HardwareManager* hw, SerialManager* sm, OperationTask* ot) : currentState(WORKING), stateStartTime(0) {
    hardware = hw;
    serial = sm;
    operationtask=ot;
    init();
}

void ProblemTask::init() {

}

void ProblemTask::tick() {
    //Serial.println("task problem");
    
    String command = serial->getCommandForProblemTask();
    

    switch (currentState) {
        case WORKING:
            working();
            break;

        case PROBLEM:
            problem(command);
            break;
    }
}

void ProblemTask::working(){
    //Serial.println("No problems");
    if (hardware->getTemperature() > TEMP_MAX && operationtask->getProblemFlag()==false) {
        Serial.println("High Temperature!!! wait for restore command");
        currentState=PROBLEM;
        operationtask->setProblemFlag(true);
        return;
    }
}

void ProblemTask::problem(String command) {
   if (command == "RESTORE") {
        Serial.println("ProblemTask: RESTORE command received. Returning to WORKING state.");
        currentState = WORKING;
        stateStartTime = millis();
        return;
    }
}