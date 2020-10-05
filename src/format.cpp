
#include <string>

#include "format.h"
#include <sstream>

using std::string;
using std::stringstream;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long int seconds) { 
  long int hour, min, sec, sec_rem;
  std::stringstream ss; 
  hour = seconds/3600;
  sec_rem = seconds%3600;
  
  min = sec_rem/60;
  sec = sec_rem%60;
  
  ss << hour << ":" << min << ":" << sec;
  return ss.str(); 
}
