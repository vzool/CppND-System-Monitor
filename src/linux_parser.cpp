#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, kernel;
  string line, version;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {

  /*
    MemTotal:       16275152 kB
    MemFree:         5207092 kB
    MemAvailable:   11821888 kB
    Buffers:            3260 kB
  */

  long int mem_total = 0, mem_free = 0, mem_available = 0, buffers = 0;

  string line, type, unit;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      if (line.find("MemTotal") != string::npos) {
        linestream >> type >> mem_total >> unit;
      } else if (line.find("MemFree") != string::npos) {
        linestream >> type >> mem_free >> unit;
      } else if (line.find("MemAvailable") != string::npos) {
        linestream >> type >> mem_available >> unit;
      } else if (line.find("Buffers") != string::npos) {
        linestream >> type >> buffers >> unit;
      }else{
        break;
      }
    }
  }
  return float(mem_total - mem_free) / float(mem_total);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  /* 2153.13 24797.00 */
  long system_time = 0, idle_process = 0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> system_time >> idle_process;
  }
  return system_time;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  /*
    cpu  1 (systemd) S 0 1 1 0 -1 4194560 8833 163604 40 121 37 68 796 1506 20 0
    1 0 22 176472064 2616 18446744073709551615 1 1 0 0 0 0 671173123 4096 1260 0
    0 0 17 0 0 0 8 0 0 0 0 0 0 0 0 0 0
  */
  string line, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x18, x19,
      x20, x21;
  long x14, x15, x16, x17, x22;
  long utime, stime, cutime, cstime, starttime;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> x1 >> x2 >> x3 >> x4 >> x5 >> x6 >> x7 >> x8 >> x9 >> x10 >>
          x11 >> x12 >> x13 >> x14 >> x15 >> x16 >> x17 >> x18 >> x19 >> x20 >>
          x21 >> x22;

      utime = x14;
      stime = x15;
      cutime = x16;
      cstime = x17;
      starttime = x22;

      return utime + stime + cutime + cstime + starttime;
    }
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> values = LinuxParser::CpuUtilization();
  return (stof(values[kUser_]) + stof(values[kNice_]) + stof(values[kSystem_]) +
          stof(values[kIdle_]) + stof(values[kSoftIRQ_]) +
          stof(values[kSteal_]) + stof(values[kGuest_]) +
          stof(values[kGuestNice_]));
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> values = LinuxParser::CpuUtilization();
  return (stof(values[kIdle_]) + stof(values[kIOwait_]));
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  /*
    cpu  222166 6282 75068 5498549 4802 9988 4985 0 0 0
    ...
  */
  vector<string> result;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line, type;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> type >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  result.push_back(std::to_string(user));
  result.push_back(std::to_string(nice));
  result.push_back(std::to_string(system));
  result.push_back(std::to_string(idle));
  result.push_back(std::to_string(iowait));
  result.push_back(std::to_string(irq));
  result.push_back(std::to_string(softirq));
  result.push_back(std::to_string(steal));
  result.push_back(std::to_string(guest));
  result.push_back(std::to_string(guest_nice));
  return result;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {

  /*
    processes 9326
  */
  long int processes;

  string line, key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (line.find("processes") != string::npos) {
        std::istringstream linestream(line);
        linestream >> key >> processes;
        break;
      }
    }
  }
  return processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  /*
    procs_running 1
  */
  int procs_running;

  string line, key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (line.find("procs_running") != string::npos) {
        std::istringstream linestream(line);
        linestream >> key >> procs_running;
        break;
      }
    }
  }
  return procs_running;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    if(std::getline(stream, line)){
      return line;
    }
  }
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  /*
    VmSize:   172336 kB
  */
  string line, key, unit;
  long size = 0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (line.find("VmSize") != string::npos) {
        std::istringstream linestream(line);
        linestream >> key >> size >> unit;
        break;
      }
    }
  }
  return std::to_string(size/1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  /*
    Uid:    0       0       0       0
    Gid:    0       0       0       0
  */
  string line, key, uid, x1, x2, x3;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (line.find("Uid") != string::npos) {
        std::istringstream linestream(line);
        linestream >> key >> uid >> x1 >> x2 >> x3;
        break;
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  auto uid = Uid(pid) + ":";
  string line, user;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      auto index = line.find(uid);
      if (index != string::npos) {
        return line.substr(0, index - 3);
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  /*
    1 (systemd) S 0 1 1 0 -1 4194560 8833 163604 40 121 37 68 796 1506 20 0 1 0 22 176472064 2616 18446744073709551615 1 1 0 0 0 0 671173123 4096 1260 0 0 0 17 1 0 0 8 0 0 0 0 0 0 0 0 0 0
  */
  string line, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21;
  long x22;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> x1 >> x2 >> x3 >> x4 >> x5 >> x6 >> x7 >> x8 >> x9 >> x10 >> x11 >> x12 >> x13 >> x14 >> x15 >> x16 >> x17 >> x18 >> x19 >> x20 >> x21 >> x22;
      return x22;
    }
  }
  return 0;
}
