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
    bool emptyFlag;
    unsigned long stateStartTime;
    const unsigned long openTime = 10000; //time the bin should remain open without any user interaction (close button click)
    const unsigned long closeTime = 3000; //time to wait after door close
    const unsigned long emptyingTime = 3000; //time to wait while emptying
    int emptyWasteLevel;
    const int maximumWasteLevel = 5; //da decidere
    const int minimumWasteLevel = 30; // quando arriva a questa distanza il wasteLevel è a 0
    void idle();
    void open();
    void emptying();
    void full();
    void close();
    void sleeping();
    void sendWasteLevelInPercentage();

public:
    void emptyContainer(); //set full flag to true
    void setProblemFlag(bool value);    //set problem flag
    bool getProblemFlag();
    OperationTask(HardwareManager* hw, SerialManager* sw);                  // Costruttore
    void init(); // Inizializza la OperationTask
    void tick();          // Aggiorna la macchina a stati
};

#endif