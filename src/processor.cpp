#include "processor.h"

#include "linux_parser.h"
#include "unistd.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  double smJiffie;
  long activeJiffies = LinuxParser::ActiveJiffies();
  long totalJiffies = LinuxParser::Jiffies();
  smJiffie = double(activeJiffies - pActive) / double(totalJiffies - pTotal);
  pActive = activeJiffies;
  pTotal = totalJiffies;
  return smJiffie;
}
 