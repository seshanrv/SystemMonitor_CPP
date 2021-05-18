#include <math.h>
#include <string.h>
#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string hr, min, sec, hhmmss;
  hr = std::to_string(seconds / 3600);
  min = std::to_string((seconds % 3600) / 60);
  sec = std::to_string(seconds % 60);
  hhmmss = hr + ":" + min + ":" + sec;
  return hhmmss;
}