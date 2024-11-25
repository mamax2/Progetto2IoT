#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

#include "HardwareManager.h"

/**
 * @brief Interfaccia della classe di gestione dell'interfaccia.
 */
class SerialManager {
private:
    HardwareManager* hardware;

public:
    void init();
    void sendTemperature(float temperature);
    void sendLevelOfWaste(int levelOfWaste);
    void handleCommand(String command);
    String getCommandForOperationTask();
    String getCommandForProblemTask();
};

#endif
