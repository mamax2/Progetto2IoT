#include <Arduino.h>
#include "headers/Scheduler.h"
#include "headers/OperationTask.h"
#include "headers/ProblemTask.h"

Scheduler sched;

void setup(){
  
  Serial.begin(9600);
  sched.init(20);
 
  Task* t0 = new OperationTask();
  t0->init(20);

  Task* t1 = new ProblemTask();
  t1->init(100);
  
  sched.addTask(t0);
  sched.addTask(t1);
}

void loop(){
  sched.schedule();
}