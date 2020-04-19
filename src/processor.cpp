#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <string>
#include <fstream>

using std::string;

void Processor::setPrevious(double user, double nice, double system, double idle, double iowait, double irq, double softirq, double steal) {
  prevuser_ = user;
  prevnice_ = nice;
  prevsystem_ = system;
  previdle_ = idle;
  previowait_ = iowait;
  previrq_ = irq;
  prevsoftirq_ = softirq;
  prevsteal_ = steal;
}

// DONE: Return the aggregate CPU utilization
double Processor::Utilization() {
  string line, cpu;
  double user, nice, system, idle, iowait, irq, softirq, steal;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }

  double PrevIdle = previdle_ + previowait_;
  double Idle = idle + iowait;

  double PrevNonIdle = prevuser_ + prevnice_ + prevsystem_ + previrq_ + prevsoftirq_ + prevsteal_;
  double NonIdle = user + nice + system + irq + softirq + steal;

  double PrevTotal = PrevIdle + PrevNonIdle;
  double Total = Idle + NonIdle;

  double totald = Total - PrevTotal;
  double idled = Idle - PrevIdle;

  double CPU_Percentage = (totald - idled)/totald;
  setPrevious(user, nice, system, idle, iowait, irq, softirq, steal);
  return CPU_Percentage;
}