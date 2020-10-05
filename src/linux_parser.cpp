#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
//using namespace std;
 

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;
using std::istringstream;


// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >>version >>kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
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

//  Read and return the system memory utilization

float LinuxParser::MemoryUtilization() {
float totalmemory{0.0}, buffer{0.0},memoryfree{0.0},value{0.0};
string line, key;
std::ifstream filestream(kProcDirectory + kMeminfoFilename);
if(filestream.is_open()){
  while(std::getline(filestream, line)){
  std::istringstream linestream(line);
    while (linestream >> key >> value){
      if (key == "MemTotal:"){
      totalmemory = value;     
      }
      else if(key == "MemFree:"){
      memoryfree = value;      
      }
      else  if(key == "Buffers:"){
      buffer = value;    
      } 
    }
  }
}
return 1.0 -(memoryfree/(totalmemory - buffer));} 

// the system uptime
// proc/uptime
long LinuxParser::UpTime() { 
  string time_str, line;
  long UpTime{0};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream stream(line);
    if (stream >> time_str){UpTime = stof(time_str);}
  }
    return UpTime; 
}

// jiffies for the system
// proc/stat
long LinuxParser::Jiffies() {
  return (ActiveJiffies() + IdleJiffies());
}


//number of active jiffies for a PID
 
long LinuxParser::ActiveJiffies(int pid) {
  	vector<string> pid_stats = CpuUtilization(pid);
  	return std::stol(pid_stats[13]) + std::stol(pid_stats[14]) + 
    std::stol(pid_stats[15]) + std::stol(pid_stats[16]);
    }
vector<string> LinuxParser::CpuUtilization(int pid) {
	std::ifstream pidFile(kProcDirectory + std::to_string(pid) + kStatFilename);
  	if (pidFile) {
      string pidLine;
      getline(pidFile, pidLine);
      pidFile.close();
      
      std::istringstream pidStream(pidLine);
      std::vector<string> pidTokens;
      string pidToken;
      
      while(pidStream >> pidToken) {
      	pidTokens.push_back(pidToken);
      }
      return pidTokens;
    }
  	return {};
}

long LinuxParser::ActiveJiffies() { 
  long activeJiffies;
  vector<CPUStates> activeStates{kUser_,kNice_,kSystem_,kIRQ_,kSoftIRQ_,kSteal_};
  vector<string> utilization=CpuUtilization();
  for (auto state: activeStates){ activeJiffies+=stol(utilization[state]);}
  return activeJiffies;
}

long LinuxParser::IdleJiffies() { 
  long idleJiffies;
  vector<CPUStates> idleStates {kIdle_,kIOwait_}
  ;
   vector<string> utilization=CpuUtilization();
  for (auto state: idleStates){ idleJiffies+=stol(utilization[state]);}
  return idleJiffies;
}

//  Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
   string line, value;
   vector<string> utilizationData{};
   std::ifstream stream(kProcDirectory+kStatFilename);
   if(stream.is_open()){
     std::getline(stream, line);
     std::istringstream linestream(line);
     while(linestream>>value){
       if(value!="cpu"){utilizationData.emplace_back(value);}
     };
   }   
return utilizationData; }

// the total number of processes/ running procs
// /proc/stat
int LinuxParser::TotalProcesses() {
  string line, key;
  int processT, value;
  ifstream Fstream(kProcDirectory+kStatFilename);
  if(Fstream.is_open()){
    while(getline(Fstream,line)){
      istringstream stream(line);
      while(stream>>key>>value){
        if (key=="processes"){ processT=value;}
      }
    }
  }  
  return processT; 
}

int LinuxParser::RunningProcesses() {
  string line, key;
  int processR, value;
  ifstream Fstream(kProcDirectory+kStatFilename);
  if(Fstream.is_open()){
    while(getline(Fstream,line)){
      istringstream stream(line);
      while(stream>>key>>value){
        if (key=="procs_running"){ 
          processR=value;
        }
      }
    }
  }  
  return processR; 
}

//
//
// Data associated with a process
// /proc/[PID]/ . . .
string LinuxParser::Command(int pid) {
  string line;
  ifstream Fstream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if(Fstream.is_open()){
    getline(Fstream,line);
  }
  return line;
}

string LinuxParser::Ram(int pid) {
  string line, key, value;
  int RAM=0;
  ifstream Fstream (kProcDirectory+to_string(pid)+kStatusFilename);
  if(Fstream.is_open()){
    while(getline(Fstream,line)){
      std::replace(line.begin(), line.end(),':', ' ');
      istringstream stream(line);
      while(stream>>key>>value){
        if (key=="VmSize"){ 
          RAM = std::stoi(value)/1024;
          return to_string(RAM);
        }
      }
    }    
  }
  return  to_string(RAM);
}

string LinuxParser::Uid(int pid) {
  string key, value, UID, line;  
  ifstream userFile(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (userFile) {
    while(userFile.is_open()) {
      getline(userFile, line);
      istringstream userStream(line);
      userStream >> key >> value;
      if (key == "Uid:") {
        UID = value;
        userFile.close();
      }
    }
  }
  return UID;
}

string LinuxParser::User(int pid) {
  string UID = Uid(pid);
  ifstream etcPassword(kPasswordPath);
  if (etcPassword) {
    while(etcPassword.is_open()) {
      string line, user, mode, m_uid;;
      getline(etcPassword, line);
      
      istringstream lineStream(line); 
      getline(lineStream, user, ':');
      getline(lineStream, mode, ':');
      getline(lineStream, m_uid, ':');

      if (m_uid == UID) {
        etcPassword.close();
        return user;
      }
    }
    etcPassword.close();
  }
  return string();
}

long int LinuxParser::UpTime(int pid) {
  vector<std::string> pidStat = CpuUtilization(pid);
  return (stol(pidStat[21]) / sysconf(_SC_CLK_TCK)) ;
}
