#ifndef ProblemTask_H
#define ProblemTask_H

#include "HardwareManager.h"
#include "SerialManager.h"
#include "Task.h"
#include "OperationTask.h"

enum ProblemState { PROBLEM , WORKING};

class ProblemTask: public Task {
private:
    ProblemState currentState;
    HardwareManager* hardware;
    SerialManager* serial;
    OperationTask* operationtask;
    
    const float TEMP_MAX=15.0;
    unsigned long stateStartTime;
    void working();
    void problem(String command);

public:
    ProblemTask();                  // Costruttore
    void init(HardwareManager* hw, SerialManager* sm, OperationTask* ot ); // Inizializza la ProblemTask
    void tick();          // Aggiorna la macchina a stati
};

#endif
