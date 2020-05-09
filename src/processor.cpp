#include "processor.h"
#include "linux_parser.h"

#include <fstream>
#include <sstream>
#include <string>

using std::string;

// TODO: Return the aggregate CPU utilization
double Processor::Utilization() {

  float totalTime = LinuxParser::Jiffies();
  float activeTime = LinuxParser::ActiveJiffies();
  return 1.0 * (activeTime / totalTime);
}