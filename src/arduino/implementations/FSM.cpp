#include "FSM.h"

FSM::FSM() : currentState(IDLE), stateStartTime(0) {}

void FSM::init(HardwareManager* hw, SerialManager* sm) {
    hardware = hw;
    serial = sm;
}

void FSM::update() {
    switch (currentState) {
        case IDLE:
            hardware->setGreenLED(true);
            hardware->setRedLED(false);
            hardware->displayMessage("PRESS OPEN TO", "ENTER WASTE");

            if (hardware->isUserDetected()) {
                currentState = WAIT_OPEN;
            }
            break;

        case WAIT_OPEN:
            if (digitalRead(hardware->BUTTON_OPEN) == LOW) {
                hardware->openDoor();
                hardware->displayMessage("DOOR OPENED", "PRESS CLOSE");
                currentState = OPEN;
                stateStartTime = millis();
            }
            break;

        case OPEN:
            if (digitalRead(hardware->BUTTON_CLOSE) == LOW || millis() - stateStartTime > 10000) {
                hardware->closeDoor();
                hardware->displayMessage("WASTE RECEIVED", "THANK YOU!");
                delay(2000);
                currentState = IDLE;
            }
            break;

        case FULL:
            hardware->setGreenLED(false);
            hardware->setRedLED(true);
            hardware->displayMessage("CONTAINER FULL", "");
            break;

        case PROBLEM:
            hardware->setGreenLED(false);
            hardware->setRedLED(true);
            hardware->displayMessage("PROBLEM DETECTED", "WAIT FOR RESTORE");
            break;
    }
}
