#include <Arduino.h>
#include "../headers/ProblemTask.h"
#include "../headers/HardwareManager.h"

ProblemTask::ProblemTask() : currentState(WORKING), stateStartTime(0) {}

void ProblemTask::init(HardwareManager* hw, SerialManager* sm) {
    hardware = hw;
    serial = sm;
}

void ProblemTask::tick() {
    Serial.Println("task problem");
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
    //put code here
}

void ProblemTask::problem(){
    //put code here
}
