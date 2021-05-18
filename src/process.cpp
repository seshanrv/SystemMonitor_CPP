#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using LinuxParser::ProcessCPUStates;

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    vector<long> process_times = LinuxParser::ProcessTimes(pid);

    float total_time = static_cast<float> (process_times[ProcessCPUStates::kUtime_] +
                        process_times[ProcessCPUStates::kStime_] + 
                        process_times[ProcessCPUStates::kCstime_] +
                        process_times[ProcessCPUStates::kCutime_]);

    cpu_util = total_time / static_cast<float> (UpTime());

    return cpu_util; 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return this->cpu_util > a.cpu_util;
    // return CpuUtilization() < a.CpuUtilization();
}