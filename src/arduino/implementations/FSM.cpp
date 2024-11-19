#include <Arduino.h>
#include "../headers/FSM.h"
#include "../headers/HardwareManager.h"

FSM::FSM() : currentState(IDLE), stateStartTime(0) {}

void FSM::init(HardwareManager* hw, SerialManager* sm) {
    hardware = hw;
    serial = sm;
}

void FSM::tick() {
    switch (currentState) {
        case IDLE:
            hardware->setGreenLED(true);
            hardware->setRedLED(false);
            hardware->displayMessage("PRESS OPEN TO", "ENTER WASTE");

            /*
            if (!hardware->isUserDetected()) {
                currentState = SLEEPING;
            }
            */
            break;

        case OPEN:
            if (digitalRead(hardware->BUTTON_OPEN) == LOW) {
                hardware->openDoor();
                hardware->displayMessage("DOOR OPENED", "PRESS CLOSE");
                currentState = OPEN;
                stateStartTime = millis();
            }
            break;

        case SLEEPING:
            /*
            if (hardware->isUserDetected()) {
                currentState = IDLE;
            }
            */
            break;

        case CLOSE:
            if (digitalRead(hardware->BUTTON_CLOSE) == LOW || millis() - stateStartTime > 10000) {
                hardware->closeDoor();
                hardware->displayMessage("WASTE RECEIVED", "THANK YOU!");
                delay(2000);
                currentState = IDLE;
            }
            break;

        case EMPTYING:
                hardware->reverseDoor();
                currentState=IDLE;

            break;

        case FULL:
            hardware->setGreenLED(false);
            hardware->setRedLED(true);
            hardware->displayMessage("CONTAINER FULL", "");

            break;
    }
}
