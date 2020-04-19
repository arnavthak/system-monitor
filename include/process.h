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
};

#endif