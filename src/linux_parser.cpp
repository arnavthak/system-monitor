#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string key;
  float memTotal, memFree;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "MemTotal:") {
        linestream >> memTotal;
      } else if (key == "MemFree:") {
        linestream >> memFree;
      }
    }
  }
  return (memTotal - memFree)/memTotal;
}

long LinuxParser::UpTime() {
  long upTime;
  string in;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, in);
    std::istringstream instream(in);
    instream >> upTime;
  }
  return upTime;
}

int LinuxParser::TotalProcesses() {
  string processes;
  int totalProcesses;
  string in;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, in)) {
      std::istringstream instream(in);
      instream >> processes;
      if (processes == "processes") {
        instream >> totalProcesses;
        return totalProcesses;
      }
    }
  }
}

int LinuxParser::RunningProcesses() {
  string key;
  int runningProcesses;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> runningProcesses;
        return runningProcesses;
      }
    }
  }
}

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  std::getline(stream, line);
  return line;
}

string LinuxParser::Ram(int pid) {
  int memory;
  string key;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:") {
        linestream >> memory;
      }
    }
  }
  int smemory = memory*.001;
  return to_string(smemory);
}

string LinuxParser::User(int pid) {
  string line;
  string key;
  int uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream >> uid;
      }
    }
  }
  string newline;
  string name, x;
  int uid_test;
  std::ifstream username(kPasswordPath);
  if (username.is_open()) {
    while (std::getline(username, newline)) {
      std::replace(newline.begin(), newline.end(), ':', ' ');
      std::istringstream namestream(newline);
      namestream >> name >> x >> uid_test;
      if (uid == uid_test) {
        return name;
      }
    }
  }
}

long LinuxParser::UpTime(int pid) {
  string line, val;
  int count = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (count < 22) {
        linestream >> val;
        count++;
      }
    }
  }
  return std::stol(val, nullptr, 10)/sysconf(_SC_CLK_TCK);
}