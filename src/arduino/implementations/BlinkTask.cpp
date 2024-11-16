#include <Arduino.h>
#include "../headers/BlinkTask.h"

BlinkTask::BlinkTask(int pin) {
  this->pin = pin;
}

void BlinkTask::init(int period) {
  Task::init(period);
  led = new Led(pin);
  state = OFF;
}

void BlinkTask::tick() {
  switch (state) {
    case OFF:
      led->switchOn();
      Serial.println("LED switched ON");  // Messaggio per lo stato ON
      state = ON;
      break;
    case ON:
      led->switchOff();
      Serial.println("LED switched OFF"); // Messaggio per lo stato OFF
      state = OFF;
      break;
  }
}