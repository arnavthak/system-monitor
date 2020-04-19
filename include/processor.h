#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  double Utilization();  // DONE: See src/processor.cpp

  // DONE: Declare any necessary private members
 private:
   double prevuser_, prevnice_, prevsystem_, previdle_, previowait_, previrq_, prevsoftirq_, prevsteal_;
   void setPrevious(double user, double nice, double system, double idle, double iowait, double irq, double softirq, double steal);
};

#endif