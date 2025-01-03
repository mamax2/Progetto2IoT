#include <Arduino.h>
#include "../headers/OperationTask.h"
#include "../headers/HardwareManager.h"

OperationTask::OperationTask(HardwareManager* hw, SerialManager* sw) : currentState(IDLE), stateStartTime(0) {
    hardware = hw;
    serial = sw;
    init();
}

void OperationTask::init() {
    setupFlag = true; //flag usato per il setup degli stati, se lo stato ha bisogno di scrivere su lcd e accendere/spegnere led
    problemFlag = false; //flag usato per bloccare la task quando si verifica un problema
    hardware->update();
}

void OperationTask::tick() {
    hardware->update();
    if(millis()%3==0){
        serial->sendTemperature(hardware->getTemperature());
    }
    if(problemFlag){
        return;
    }
    switch (currentState) {
        case IDLE:
            idle();
            break;
        case OPEN:
            sendWasteLevelInPercentage();
            open();
            break;
        case SLEEPING:
            sleeping();
            break;
        case CLOSE:
            close();
            break;

        case EMPTYING:
            sendWasteLevelInPercentage();
            emptying();
            break;

        case FULL:
            full();
            break;
    }
}

//idle state, displaying message to the user "press open...", waiting for user click to open the bin and checking if user is in front of the bin
void OperationTask::idle() {
    if(hardware->isUserDetected()){
        if(setupFlag)
        {   //setUp task idle
            hardware->setGreenLED(HIGH);
            hardware->setRedLED(LOW);
            hardware->displayMessage("PRESS OPEN", "ENTER WASTE");
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

    if((millis() - stateStartTime > openTime) || digitalRead(hardware->BUTTON_CLOSE))
    {   //if "openTime" seconds pass without user interaction (button close click) the door automatically close or the user click button close
        currentState = CLOSE;
        setupFlag = true;
    }
    
    if(hardware->getWasteLevel() <= maximumWasteLevel && millis() - stateStartTime > 1000)
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

    if(millis() - stateStartTime > closeTime)
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

    serial->handleCommand(Serial.readStringUntil('\n'));
    if(serial->getCommandForOperationTask() == "EMPTY")
    {   //if empty flag is turned true by the operator from the dashboard the container go to emptying state
        currentState = EMPTYING;
        setupFlag = true;
        emptyFlag = false;
    }
}

//function used to access problem flag from problemTask
void OperationTask::setProblemFlag(bool value){
    problemFlag = value;
    if(!value){
        setupFlag = true;
    }
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

void OperationTask::sendWasteLevelInPercentage(){
    int wasteLevel = hardware->getWasteLevel();
    if(wasteLevel >= minimumWasteLevel){
        serial->sendLevelOfWaste(0);
    }else if(wasteLevel <= maximumWasteLevel){
        serial->sendLevelOfWaste(100);
    }else{
        int temp = 100 - ((100 / (minimumWasteLevel - maximumWasteLevel)) * wasteLevel);
        if(temp > 100){
            serial->sendLevelOfWaste(100);
        } else if(temp<0) {
            serial->sendLevelOfWaste(0);
        }else{
            serial->sendLevelOfWaste(temp);
        }
        
    }
    
}