#include <string>

#include "format.h"

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) {
  string elapsedTime;
  int formatSeconds = seconds % 60;
  int formatMinutes = (seconds/60) % 60;
  int formatHours = (seconds/3600) % 60;
  if (to_string(formatHours).length() == 1) {
    elapsedTime += "0" + to_string(formatHours);
  } else {
    elapsedTime += to_string(formatHours);
  }
  elapsedTime += ":";
  if (to_string(formatMinutes).length() == 1) {
    elapsedTime += "0" + to_string(formatMinutes);
  } else {
    elapsedTime += to_string(formatMinutes);
  }
  elapsedTime += ":";
  if (to_string(formatSeconds).length() == 1) {
    elapsedTime += "0" + to_string(formatSeconds);
  } else {
    elapsedTime += to_string(formatSeconds);
  }
  return elapsedTime; 
}