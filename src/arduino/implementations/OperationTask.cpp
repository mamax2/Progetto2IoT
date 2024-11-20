#include <Arduino.h>
#include "../headers/OperationTask.h"
#include "../headers/HardwareManager.h"

OperationTask::OperationTask() : currentState(IDLE), stateStartTime(0) {}

void OperationTask::init(HardwareManager* hw, SerialManager* sm) {
    hardware = hw;
    serial = sm;
}

void OperationTask::tick() {
    switch (currentState) {
        case IDLE:
            idle();
            break;

        case OPEN:
            open();
            break;

        case SLEEPING:
            sleeping();
            break;

        case CLOSE:
            close();
            break;

        case EMPTYING:
            emptying();
            break;

        case FULL:
            full();
            break;
    }
}

void idle(){
    if(hardware->isUserDetected()){
        hardware->setGreenLED(true);
        hardware->setRedLED(false);
        hardware->displayMessage("PRESS OPEN TO", "ENTER WASTE");
    }
    else{
        currentState = SLEEPING;
    }
}

void open(){
    if (digitalRead(hardware->BUTTON_OPEN) == LOW && hardware->isUserDetected()) {
        hardware->openDoor();
        hardware->displayMessage("DOOR OPENED", "PRESS CLOSE");
        currentState = OPEN;
        stateStartTime = millis();
    }
}

void sleeping(){
    if (hardware->isUserDetected()) {
        currentState = IDLE;
    }
}

void close(){
    if (digitalRead(hardware->BUTTON_CLOSE) == LOW || millis() - stateStartTime > 10000) {
        hardware->closeDoor();
        hardware->displayMessage("WASTE RECEIVED", "THANK YOU!");
        delay(2000);
        currentState = IDLE;
    }
}

void emptying(){
    hardware->reverseDoor();
    currentState=IDLE;
}

void full(){
    hardware->setGreenLED(false);
    hardware->setRedLED(true);
    hardware->displayMessage("CONTAINER FULL", "");
}