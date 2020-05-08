#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  void Pid(int);
  void User(std::string);
  void Command(std::string);
  void CpuUtilization(float);
  void Ram(std::string);
  void UpTime(long int);

  // TODO: Declare any necessary private members
 private:
 int pid_;
 float cpu_;
 long int uptime_;
 std::string user_, command_, ram_;
};

#endif