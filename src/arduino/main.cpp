
#include <Arduino.h>
#include "headers/Scheduler.h"
#include "headers/OperationTask.h"
#include "headers/ProblemTask.h"
#include "headers/SerialManager.h"

Scheduler sched;
SerialManager serialManager;


void setup(){
  
  sched.init(20);
 
  Task* t0 = new OperationTask();
  t0->init(20);

  Task* t1 = new ProblemTask();
  t1->init(100);
  
  sched.addTask(t0);
  sched.addTask(t1);
  serialManager.init();
}

void loop(){
  sched.schedule();
}