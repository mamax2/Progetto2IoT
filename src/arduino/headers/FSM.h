#ifndef FSM_H
#define FSM_H

#include "HardwareManager.h"
#include "SerialManager.h"

enum State { IDLE, SLEEPING, OPEN, CLOSE, FULL, EMPTYING};

class FSM : public Task {
private:
    State currentState;
    HardwareManager* hardware;
    SerialManager* serial;
    unsigned long stateStartTime;

public:
    FSM();                  // Costruttore
    void init(HardwareManager* hw, SerialManager* sm); // Inizializza la FSM
    void tick();          // Aggiorna la macchina a stati
};

#endif
