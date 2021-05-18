#include <dirent.h>
#include <unistd.h>
#include <unistd.h>
#include <string>
#include <vector>
// #include <iostream>
#include <iomanip>

#include "format.h"
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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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
        pids.emplace_back(stoi(filename));
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key;
  float mem_total, mem_free, value;
  std::ifstream mem_file(kProcDirectory + kMeminfoFilename);
  if (mem_file.is_open()) {
    while (std::getline(mem_file, line)) {
      std::istringstream line_stream(line);
      line_stream >> key >> value;
      if (key == "MemTotal:") {
        mem_total = value;
      } else if (key == "MemAvailable:") {
        mem_free = value;
      }
    }
  }
  return (mem_total - mem_free) / mem_total;
}

// TODO: Read and return the system uptime
long int LinuxParser::UpTime() {
  long int sys_uptime;
  string line;
  std::ifstream uptime_file(kProcDirectory + kUptimeFilename);
  if (uptime_file.is_open()) {
    std::getline(uptime_file, line);
    std::istringstream line_stream(line);
    line_stream >> sys_uptime;
  }
  return sys_uptime;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function

vector<long> LinuxParser::ProcessTimes(int pid) {
  string line;
  string word;
  vector<int> fields{14, 15, 16, 17, 22};
  vector<long> process_times;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    unsigned int idx = 0;
    int n = 1;
    long clk_tck = sysconf(_SC_CLK_TCK);

    while (linestream >> word) {
      if (n == fields[idx]) {
        process_times.push_back(stol(word) / clk_tck);
        if (idx == fields.size() - 1) {
          break;
        }
        idx++;
      }
      n++;
    }
  }
  return process_times;
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  vector<long> cpu_util;
  long value;
  string line, key;
  std::ifstream stat_file(kProcDirectory + kStatFilename);
  while (std::getline(stat_file, line)) {
    std::istringstream line_stream(line);
    line_stream >> key;
    if (key == "cpu") {
      for (int i = 0; i < 10; i++) {
        line_stream >> value;
        cpu_util.emplace_back(value);
      }
      break;
    }
  }
  return cpu_util;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int total_proc;
  std::ifstream stat_file(kProcDirectory + kStatFilename);
  if (stat_file.is_open()) {
    while (std::getline(stat_file, line)) {
      std::istringstream line_stream(line);
      line_stream >> key;
      if (key == "processes") {
        line_stream >> total_proc;
        break;
      }
    }
  }
  return total_proc;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int running_proc;
  std::ifstream stat_file(kProcDirectory + kStatFilename);
  if (stat_file.is_open()) {
    while (std::getline(stat_file, line)) {
      std::istringstream line_stream(line);
      line_stream >> key;
      if (key == "procs_running") {
        line_stream >> running_proc;
        break;
      }
    }
  }
  return running_proc;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string cmd;
  std::ifstream cmdline_file(kProcDirectory + std::to_string(pid) +
                             kCmdlineFilename);
  if (cmdline_file.is_open()) {
    std::getline(cmdline_file, cmd);
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        std::ostringstream val_stream;
        val_stream << std::fixed << std::setprecision(1) << stof(value) / 1000;
        value = val_stream.str();
        break;
      }
    }
  }
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        break;
      }
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line;
  string username, x, user_id;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> x >> user_id;
      if (uid == user_id) {
        break;
      }
    }
  }
  return username;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string word;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int n = 1;
    while (linestream >> word) {
      if (n == 22) {
        break;
      }
      n++;
    }
  }
  return UpTime() - std::stol(word) / sysconf(_SC_CLK_TCK);
}
