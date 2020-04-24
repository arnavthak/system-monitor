#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

int Process::Pid() { return pid_; }

double Process::CpuUtilization() {
  string temp;
  string line;
  int count = 1;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    while (linestream >> temp) {
      if (count == 14) {
        utime_ = stof(temp);
      } else if (count == 15) {
        stime_ = stof(temp);
      } else if (count == 16) {
        cutime_ = stof(temp);
      } else if (count == 17) {
        cstime_ = stof(temp);
      } else if (count == 22) {
        starttime_ = stof(temp);
      }
      count++;
    }
  }
  long uptime = LinuxParser::UpTime();
  double total_time = utime_ + stime_ + cutime_ + cstime_;

  double seconds = uptime - (starttime_ / sysconf(_SC_CLK_TCK));
  double cpu_usage = (total_time / sysconf(_SC_CLK_TCK)) / seconds;

  return cpu_usage;
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator>(Process& a) {
  if (a.CpuUtilization() < CpuUtilization()) {
    return true;
  } else {
    return false;
  }
}