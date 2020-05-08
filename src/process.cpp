#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  cpu_ = float(LinuxParser::ActiveJiffies(pid_)) / float(LinuxParser::ActiveJiffies());
  return cpu_;
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

void Process::Pid(int x){ pid_ = x; }
void Process::User(std::string x){ user_ = x; }
void Process::Command(std::string x){ command_ = x; }
void Process::CpuUtilization(float x){ cpu_ = x; }
void Process::Ram(std::string x){ ram_ = x; }
void Process::UpTime(long int x){ uptime_ = x; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return cpu_ > a.cpu_;
}