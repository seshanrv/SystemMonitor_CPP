#include <thread>
#include <chrono>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using LinuxParser::CPUStates;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {

  vector<long> cpu_util_prev = LinuxParser::CpuUtilization();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  vector<long> cpu_util = LinuxParser::CpuUtilization();

  long PrevIdle = cpuIdle(cpu_util_prev);
  long Idle = cpuIdle(cpu_util);

  long PrevNonIdle = cpuNonIdle(cpu_util_prev);
  long NonIdle = cpuNonIdle(cpu_util);

  long PrevTotal = PrevIdle + PrevNonIdle;
  long Total = Idle + NonIdle;

  //   differentiate:  actual value minus the previous one
  long totald = Total - PrevTotal;
  long idled = Idle - PrevIdle;

  float CPU_Percentage = (totald - idled) / totald;

  return CPU_Percentage;
}

long Processor::cpuIdle(vector<long> cpu_util){
    return cpu_util[CPUStates::kIdle_] + cpu_util[CPUStates::kIOwait_]; 
}

long Processor::cpuNonIdle(vector<long> cpu_util){
    return cpu_util[CPUStates::kUser_] + cpu_util[CPUStates::kNice_] + cpu_util[CPUStates::kSoftIRQ_] 
            + cpu_util[CPUStates::kIRQ_] + cpu_util[CPUStates::kSteal_];
}