#ifndef FSM_H
#define FSM_H

#include "HardwareManager.h"
#include "SerialManager.h"

enum State { IDLE, WAIT_OPEN, OPEN, WAIT_CLOSE, FULL, PROBLEM };

class FSM {
private:
    State currentState;
    HardwareManager* hardware;
    SerialManager* serial;
    unsigned long stateStartTime;

public:
    FSM();                  // Costruttore
    void init(HardwareManager* hw, SerialManager* sm); // Inizializza la FSM
    void update();          // Aggiorna la macchina a stati
};

#endif
