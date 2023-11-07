#ifndef INFOACQUISITION_HPP
#define INFOACQUISITION_HPP

#include <string>
#include <vector>

struct ProcessInfo {
    int pid;
    std::string name;
    std::string status;
    double cpu_usage;
};

std::vector<ProcessInfo> ReadProcFileSystem();

double GetProcessCpuUsage(int pid);

double GetTotalCpuTime();



#endif