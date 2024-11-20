#ifndef OperationTask_H
#define OperationTask_H

#include "HardwareManager.h"
#include "SerialManager.h"

enum State { IDLE, SLEEPING, OPEN, CLOSE, FULL, EMPTYING};

class OperationTask : public Task {
private:
    State currentState;
    HardwareManager* hardware;
    SerialManager* serial;
    unsigned long stateStartTime;
    void idle();
    void open();
    void emptying();
    void full();
    void close();
    void sleeping();

public:
    OperationTask();                  // Costruttore
    void init(HardwareManager* hw, SerialManager* sm); // Inizializza la OperationTask
    void tick();          // Aggiorna la macchina a stati
};

#endif
