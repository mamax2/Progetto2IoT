#ifndef HARDWARE_MANAGER_H
#define HARDWARE_MANAGER_H

#include <ServoTimer2.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <pins_arduino.h>
#include <Wire.h>

class HardwareManager {
private:
    ServoTimer2 doorServo;
    LiquidCrystal lcd;
    float temperature;
    int wasteLevel;
    bool userDetected;

public:
    // Pin e componenti
    const int PIR_PIN=5;
    const int BUTTON_OPEN = 8;
    const int BUTTON_CLOSE = 10;
    const int TEMP_SENSOR = A2;
    const int ECHO_WASTE_SENSOR = 7;
    const int TRIG_WASTE_SENSOR = 6;
    const int GREEN_LED = 12;
    const int RED_LED = 11;
    const int RS_PIN=4;
    const int EN=2;
    const int D4=A1;
    const int D5=A3;
    const int D6=A4;
    const int D7=A5;

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
    void reverseDoor();

    // Controllo LCD
    void displayMessage(String line1, String line2);
};

#endif
