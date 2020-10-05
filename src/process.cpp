#include <unistd.h> //sysconf()
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include <linux_parser.h>
#include "process.h"
#include "processor.h"


using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid):pid_(pid){}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
  unsigned long int activeJiffies = LinuxParser::ActiveJiffies(Pid());
  unsigned long int processUptime = LinuxParser::UpTime(Pid());
  unsigned long int systemUptime = LinuxParser::UpTime();
  auto seconds = systemUptime - processUptime;
  if(seconds <= 0) return 0.0;
  return (float) (1.0*(activeJiffies/sysconf(_SC_CLK_TCK)) / seconds);   
}

string Process::Command() {
  return LinuxParser::Command(Pid());
}

string Process::Ram() { 
  return LinuxParser::Ram(Pid());
}

string Process::User() { 
  return LinuxParser::User(Pid());
}

long int Process::UpTime() {
    return LinuxParser::UpTime(Pid());
}

bool Process::operator<(Process const& a) const {
  return cpuUtilization_ > a.cpuUtilization_; 
}