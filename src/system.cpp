#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"


using std::set;
using std::size_t;
using std::string;
using std::vector;
using namespace::std;

System::System(){
  for (auto pid : LinuxParser::Pids()){
    allProcesses_.push_back(Process(pid));
  }
}
  
Processor& System::Cpu() { 
  vector<string> utilizations = LinuxParser::CpuUtilization();
  utilizations.clear();
  return cpu_; 
}

vector<Process>& System::Processes() { 
    allProcesses_.clear();
  vector<int> pids = LinuxParser::Pids();
  for (auto pid : pids){
    allProcesses_.emplace_back(Process(pid));
  }
  std::sort(allProcesses_.begin(), allProcesses_.end());
  return allProcesses_;
 // std::sort(allProcesses_.begin(),allProcesses_.end());
//  return allProcesses_; 
}

std::string System::Kernel() { 
  return LinuxParser::Kernel(); 
}

float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem() {
  return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses();
}

int System::TotalProcesses() { 
  return allProcesses_.size();
}

long int System::UpTime() {
  return LinuxParser::UpTime();
}