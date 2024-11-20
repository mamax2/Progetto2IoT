#include <Arduino.h>
#include "../headers/ProblemTask.h"
#include "../headers/HardwareManager.h"

ProblemTask::ProblemTask() : currentState(WORKING), stateStartTime(0) {}

void ProblemTask::init(HardwareManager* hw, SerialManager* sm) {
    hardware = hw;
    serial = sm;
}

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

void working(){
    //put code here
}

void problem(){
    //put code here
}
