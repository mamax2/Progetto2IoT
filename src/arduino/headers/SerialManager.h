#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

#include "HardwareManager.h"

class SerialManager {
private:
    HardwareManager* hardware;

public:
    void init();
    void update();
};

#endif
