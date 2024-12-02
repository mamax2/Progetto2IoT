
#include <Arduino.h>
#include "headers/Scheduler.h"
#include "headers/OperationTask.h"
#include "headers/ProblemTask.h"
#include "headers/SerialManager.h"
#include "headers/HardwareManager.h"

Scheduler sched;
SerialManager serialManager;
HardwareManager* hw = new HardwareManager();

void setup(){
  
  hw->init();
  sched.init(20);
  
  serialManager.init();
 
  Task* t0 = new OperationTask(hw);
  t0->init(20);

  Task* t1 = new ProblemTask();
  t1->init(100);
  
  sched.addTask(t0);
  sched.addTask(t1);
  
}

void loop(){
  sched.schedule();
}