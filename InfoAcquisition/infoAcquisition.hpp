#ifndef INFOACQUISITION_HPP
#define INFOACQUISITION_HPP

#include <string>
#include <vector>

#include "utilities.hpp"

struct Arguments;

struct ProcessInfo {
    int pid;
    std::string name;
    std::string status;
    double cpu_usage;
    double in_traffic;
    double out_traffic;
    long int used_memory;
    std::string user;
    std::string group;
};

struct NetTraffic{
    uint in;
    uint out;
};

/**
 * @brief Read processes information from /proc filesystem
 * 
 * @param arguments The vector to retrieve only the requested arguments
 * @return std::vector<ProcessInfo> 
 */
std::vector<ProcessInfo> ReadProcFileSystem(Arguments& args);

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
 * @brief Get the system's Net Usage object
 * 
 * @param pid The process identifier
 * @return struct NetTraffic The in and out traffic of the process
 * 
 * @post (in < 0 && out < 0): error opening the file
 */
struct NetTraffic GetSystemNetUsage(int pid);

/**
 * @brief Gets RAM Usage of a process
 * 
 * @param pid The process identifier
 * @return long int The RAM usage
 */
long int getProcessRAMUsage(int pid, Arguments& args);

/**
 * @brief Gets the process' real user
 * 
 * @param pid The pid of the process
 * @return std::string The name of the real user, unkown if couldn't be found or error if couldn't open the file
 */
std::string getProcessUser(int pid);

/**
 * @brief Gets the process Group
 * 
 * @param pid The pid of the process
 * @return std::string The name of the group, unkown if couldn't be found or error if couldn't open the file
 */
std::string getProcessGroup(int pid);

#endif