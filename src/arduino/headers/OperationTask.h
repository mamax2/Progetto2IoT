#ifndef OperationTask_H
#define OperationTask_H

#include "HardwareManager.h"
#include "SerialManager.h"
#include "Task.h"

enum OperationState { IDLE, SLEEPING, OPEN, CLOSE, FULL, EMPTYING};

class OperationTask: public Task {
private:
    OperationState currentState;
    HardwareManager* hardware;
    SerialManager* serial;
    bool problemFlag;
    bool setupFlag;
    unsigned long stateStartTime;
    const unsigned long openTime = 5000; //time the bin should remain open without any user interaction (close button click)
    const unsigned long closeTime = 3000; //time to wait after door close
    int emptyWasteLevel;
    const int maximumWasteLevel = 100 //da decidere
    void idle();
    void open();
    void emptying();
    void full();
    void close();
    void sleeping();

public:
    void setProblemFlag(bool value);
    OperationTask();                  // Costruttore
    void init(HardwareManager* hw, SerialManager* sm); // Inizializza la OperationTask
    void tick();          // Aggiorna la macchina a stati
};

#endif
