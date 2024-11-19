#ifndef HARDWARE_MANAGER_H
#define HARDWARE_MANAGER_H

#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <pins_arduino.h>

class HardwareManager {
private:
    // Pin e componenti
    const int PIR_PIN=5;
    const int BUTTON_OPEN = 8;
    const int BUTTON_CLOSE = 10;
    const int TEMP_SENSOR = A2;
    const int WASTE_SENSOR = 7;
    const int GREEN_LED = 12;
    const int RED_LED = 11;
    Servo doorServo;
    LiquidCrystal_I2C lcd;

    float temperature;
    int wasteLevel;
    bool userDetected;

public:
    HardwareManager();     // Costruttore
    void init();           // Inizializza tutti i componenti
    void update();         // Aggiorna i sensori e le variabili

    // Metodi per ottenere lo stato dei sensori
    bool isUserDetected();
    float getTemperature();
    int getWasteLevel();

    // Controllo LED
    void setGreenLED(bool state);
    void setRedLED(bool state);

    // Controllo porta
    void openDoor();
    void closeDoor();

    // Controllo LCD
    void displayMessage(const char* line1, const char* line2);
};

#endif
