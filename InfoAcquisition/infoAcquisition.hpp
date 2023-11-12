#ifndef INFOACQUISITION_HPP
#define INFOACQUISITION_HPP

#include <string>
#include <vector>

struct ProcessInfo {
    int pid;
    std::string name;
    std::string status;
    double cpu_usage;
    double in_traffic;
    double out_traffic;
    long int used_memory;
};

struct NetTraffic{
    double in;
    double out;
};

/**
 * @brief Read processes information from /proc filesystem
 * 
 * @param arguments The vector to retrieve only the requested arguments
 * @return std::vector<ProcessInfo> 
 */
std::vector<ProcessInfo> ReadProcFileSystem(std::vector<int>& arguments);

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


/**
 * @brief Get the Process Net Usage object
 * 
 * @param pid The process identifier
 * @return struct NetTraffic The in and out traffic of the process
 * 
 * @post (in < 0 && out < 0): error en la apertura del fichero
 */
struct NetTraffic GetProcessNetUsage(int pid);

/**
 * @brief Gets RAM Usage of a process
 * 
 * @param pid The process identifier
 * @return long int The RAM usage
 */
long int getProcessRAMUsage(int pid);


#endif