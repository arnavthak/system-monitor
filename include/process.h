#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <string>
#include "linux_parser.h"

class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();                      
  std::string Command();                   
  double CpuUtilization();                 
  std::string Ram();                       
  long int UpTime();                       
  bool operator>(Process& a);

 private:
   int pid_;
   double utime_ = 0.0;
   double stime_ = 0.0;
   double cutime_ = 0.0;
   double cstime_ = 0.0;
   double starttime_ = 0.0;

};

#endif