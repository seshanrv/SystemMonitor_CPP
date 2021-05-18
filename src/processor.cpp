#include <thread>
#include <chrono>
//TODO: remove after debug
// #include <iostream>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using LinuxParser::CPUStates;

// TODO: Check formulae
// TODO: there is a pause in the display funciton, save prev values as class member? 
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
  float totald = static_cast<float> (Total - PrevTotal);
  float idled = static_cast<float> (Idle - PrevIdle);

  float CPU_Percentage = (totald - idled) / totald;
  // float CPU_Percentage = NonIdle / Total;
  // std::cout << "Totald" << totald << "\n";
  // std::cout << "ideld" << idled << "\n";



  return CPU_Percentage;
}

long Processor::cpuIdle(vector<long> cpu_util){
    return cpu_util[CPUStates::kIdle_] + cpu_util[CPUStates::kIOwait_]; 
}

long Processor::cpuNonIdle(vector<long> cpu_util){
    return cpu_util[CPUStates::kUser_] + cpu_util[CPUStates::kNice_] + cpu_util[CPUStates::kSoftIRQ_] 
            + cpu_util[CPUStates::kIRQ_] + cpu_util[CPUStates::kSteal_] + cpu_util[CPUStates::kSystem_]; 
}