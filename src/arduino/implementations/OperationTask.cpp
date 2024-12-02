#include <Arduino.h>
#include "../headers/OperationTask.h"
#include "../headers/HardwareManager.h"

OperationTask::OperationTask(HardwareManager* hw) : currentState(IDLE), stateStartTime(0) {
    hardware = hw;
    init();
}

void OperationTask::init() {
    Serial.println("INIT OPERATION TASK");
    setupFlag = true; //flag usato per il setup degli stati, se lo stato ha bisogno di scrivere su lcd e accendere/spegnere led
    problemFlag = false; //flag usato per bloccare la task quando si verifica un problema
    hardware->update();
    emptyWasteLevel = hardware->getWasteLevel();
}

void OperationTask::tick() {
    //Serial.println(currentState);
    hardware->update();
    //Serial.println(hardware->getWasteLevel());
    //if(problemFlag){
    //    return;
    //}
    switch (currentState) {
        case IDLE:
            Serial.println("idle task");
            idle();
            break;

        case OPEN:
            Serial.println("open task");
            open();
            break;

        case SLEEPING:
            Serial.println("sleeping task");
            sleeping();
            break;

        case CLOSE:
            Serial.println("close task");
            close();
            break;

        case EMPTYING:
            Serial.println("emptying task");
            emptying();
            break;

        case FULL:
            Serial.println("full task");
            full();
            break;
    }
    
}

//idle state, displaying message to the user "press open...", waiting for user click to open the bin and checking if user is in front of the bin
void OperationTask::idle() {
    if(true){
        if(setupFlag)
        {   //setUp task idle
            Serial.println("step1");
            hardware->setGreenLED(HIGH);
            Serial.println("step2");
            hardware->setRedLED(LOW);
            Serial.println("step3");
            hardware->displayMessage("PRESS OPEN", "ENTER WASTE");
            Serial.println("step4");
            setupFlag = false;
        }

        if(digitalRead(hardware->BUTTON_OPEN) == HIGH)
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

    if((stateStartTime + openTime < millis())/* || (digitalRead(hardware->BUTTON_CLOSE) == LOW)*/)
    {   //if "openTime" seconds pass without user interaction (button close click) the door automatically close or the user click button close
        currentState = CLOSE;
        setupFlag = true;
    }

    if(hardware->getWasteLevel() <= maximumWasteLevel)
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

    if(stateStartTime + closeTime < millis())
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
    
    if(stateStartTime + emptyingTime < millis())
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


bool OperationTask::getProblemFlag(){
    return problemFlag;
}

//function used to access empty flag from serial manager
void OperationTask::emptyContainer(){
    if(currentState == FULL){
        emptyFlag = true;
    }
}