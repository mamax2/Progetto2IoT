#include <Arduino.h>
#include "../headers/OperationTask.h"
#include "../headers/HardwareManager.h"

OperationTask::OperationTask() : currentState(IDLE), stateStartTime(0) {}

void OperationTask::init(HardwareManager* hw, SerialManager* sm) {
    hardware = hw;
    serial = sm;
    setupFlag = true; //flag usato per il setup degli stati, se lo stato ha bisogno di scrivere su lcd e accendere/spegnere led
    problemFlag = false; //flag usato per bloccare la task quando si verifica un problema
}

void OperationTask::tick() {
    Serial.println("task operation");
    if(problemFlag){
        return;
    }
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

//idle state, displaying message to the user "press open...", waiting for user click to open the bin and checking if user is in front of the bin
void OperationTask::idle(){
    if(hardware->isUserDetected()){
        if(setupFlag)
        {   //setUp task idle
            hardware->setGreenLED(true);
            hardware->setRedLED(false);
            hardware->displayMessage("PRESS OPEN TO", "ENTER WASTE");
            setupFlag = false;
        }

        if(digitalRead(hardware->BUTTON_OPEN) == LOW)
        { //if button open is pressed -> move to open state
            currentState = OPEN;
            setupFlag = true;
        }
    }
    else
    {   //if no user detected in front of the bin -> move to sleeping state    
        currentState = SLEEPING;
        setupFlag = true;
    }
}


void OperationTask::open(){
    if(setupFlag)
    {   //open state setup
        hardware->displayMessage("DOOR OPENING", "PLEASE WAIT");
        hardware->openDoor();
        hardware->displayMessage("DOOR OPENED", "PRESS CLOSE");
        setupFlag = false;
        stateStartTime = millis();
    }

    if(stateStartTime + openTime > millis() || digitalRead(hardware->BUTTON_CLOSE) == LOW)
    { // if "openTime" seconds pass without user interaction (button close click) the door automatically close or the user click button close
        currentState = close();
        setupFlag = false;
    }
}

void OperationTask::sleeping(){
    if (hardware->isUserDetected()) {
        currentState = IDLE;
    }
}

void OperationTask::close(){
    if (digitalRead(hardware->BUTTON_CLOSE) == LOW || millis() - stateStartTime > 10000) {
        hardware->closeDoor();
        hardware->displayMessage("WASTE RECEIVED", "THANK YOU!");
        delay(2000);
        currentState = IDLE;
    }
}

void OperationTask::emptying(){
    hardware->reverseDoor();
    currentState=IDLE;
}

void OperationTask::full(){
    hardware->setGreenLED(false);
    hardware->setRedLED(true);
    hardware->displayMessage("CONTAINER FULL", "");
}

void OperationTask::setProblemFlag(bool value){
    problemFlag = value;
    return;
}

void OperationTask::getProblemFlag(){
    return problemFlag;
}