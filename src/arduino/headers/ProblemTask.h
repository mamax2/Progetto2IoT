#ifndef ProblemTask_H
#define ProblemTask_H

#include "HardwareManager.h"
#include "SerialManager.h"

enum State { PROBLEM , WORKING};

class ProblemTask : public Task {
private:
    State currentState;
    HardwareManager* hardware;
    SerialManager* serial;
    unsigned long stateStartTime;
    void working();
    void problem();

public:
    ProblemTask();                  // Costruttore
    void init(HardwareManager* hw, SerialManager* sm); // Inizializza la ProblemTask
    void tick();          // Aggiorna la macchina a stati
};

#endif
