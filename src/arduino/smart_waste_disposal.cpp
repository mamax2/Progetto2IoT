/**
 * @file smart_waste_disposal.cpp
 * @author Roberto Pisu (roberto.pisu@studio.unibo.it)
 * @author Giacomo Ruscelli (giacomo.ruscelli@studio.unibo.it)
 * @author Sohail Mama (sohail.mama@studio.unibo.it)
 * @brief Assignment 2 of IoT course.
 * @version 0.1
 * @date 2024-12-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <Arduino.h>
#include "headers/Scheduler.h"
#include "headers/OperationTask.h"
#include "headers/ProblemTask.h"
#include "headers/SerialManager.h"
#include "headers/HardwareManager.h"

Scheduler sched;
SerialManager* sw;
HardwareManager* hw;
OperationTask* ot;
ProblemTask* pt;

void setup(){
  Serial.begin(9600);
  hw = new HardwareManager();
  hw->init();
  sw = new SerialManager();
  sw->init();
  sched.init(20);

  ot = new OperationTask(hw,sw);
  pt = new ProblemTask(hw,sw,ot);

 
  Task* t0 = ot;
  t0->init(20);

  Task* t1 = pt;
  t1->init(100);
  
  sched.addTask(t0);
  sched.addTask(t1);
  
}

void loop(){
  sched.schedule();
}