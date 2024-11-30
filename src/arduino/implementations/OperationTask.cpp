#include <Arduino.h>
#include "../headers/OperationTask.h"
#include "../headers/HardwareManager.h"

OperationTask::OperationTask() : currentState(IDLE), stateStartTime(0) {}

void OperationTask::init(HardwareManager* hw, SerialManager* sm) {
    hardware = hw;
    serial = sm;
    setupFlag = true; //flag usato per il setup degli stati, se lo stato ha bisogno di scrivere su lcd e accendere/spegnere led
    problemFlag = false; //flag usato per bloccare la task quando si verifica un problema
    emptyWasteLevel = hardware->getWasteLevel();
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
    {   //if "openTime" seconds pass without user interaction (button close click) the door automatically close or the user click button close
        currentState = CLOSE;
        setupFlag = true;
    }

    if(hardware->getWasteLevel() >= maximumWasteLevel)
    {   //if bin get full -> go to full state and wait for operator emptying process
        currentState = FULL;
        setupFlag = true;
    }
}

void OperationTask::sleeping(){
    if(setupFlag)
    {   //sleeping state setup, clearing lcd
        hardware->displayMessage("", "");
        setupFlag = false;
    }
    
    if (hardware->isUserDetected()) 
    {   //if user is detected go back to idle
        currentState = IDLE;
        setupFlag = true;
    }
}

void OperationTask::close(){
    if(setupFlag)
    {   //close state setup, closing door and displaying message
        hardware->displayMessage("WASTE RECEIVED","THANK YOU");
        hardware->closeDoor();
        setupFlag = false;
        stateStartTime = millis();
    }

    if(stateStartTime + closeTime > millis())
    {   //when T2 time pass the task will go back to idle
        currentState = IDLE;
        setupFlag = true;
    }
    
}

void OperationTask::emptying(){
    if(setupFlag)
    {   //emptying task setup, displaying message and reversing door
        hardware->displayMessage("EMPTYING IN","PROGRESS");
        hardware->reverseDoor();
        setupFlag = false;
        stateStartTime = millis();
    }
    
    if(stateStartTime + emptyingTime > millis())
    {   //after t3 seconds the door close and go back to idle state
        hardware->closeDoor();
        currentState=IDLE;
        setupFlag = true;
    }
}

void OperationTask::full(){
    if(setupFlag)
    {   //full state setup, displaying message and closing the door, setting the led as requested
        hardware->displayMessage("CONTAINER FULL","");
        hardware->closeDoor();
        hardware->setGreenLED(false);
        hardware->setRedLED(true);
        setupFlag = false;
    }

    if(emptyFlag)
    {   //if empty flag is turned true by the operator from the dashboard the container go to emptying state
        currentState = EMPTYING;
        setupFlag = true;
        emptyFlag = false;
    }
}

//function used to access problem flag from problemTask
void OperationTask::setProblemFlag(bool value){
    problemFlag = value;
    return;
}

//function used to access empty flag from serial manager
void OperationTask::emptyContainer(){
    if(currentState == FULL){
        emptyFlag = true;
    }
}
