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

/**
 * @brief Read processes information from /proc filesystem
 * 
 * @return std::vector<ProcessInfo> 
 */
std::vector<ProcessInfo> ReadProcFileSystem();

/**
 * @brief Get the current Process Cpu Usage of a process
 * 
 * @param pid 
 * @return double 
 */
double GetProcessCpuUsage(int pid);

/**
 * @brief Get the Total Cpu Time
 * 
 * @return double 
 */
double GetTotalCpuTime();



#endif