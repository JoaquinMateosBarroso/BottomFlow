#ifndef INFOACQUISITION_HPP
#define INFOACQUISITION_HPP

#include <string>
#include <vector>
#include <unistd.h>
#include <map>

#include "utilities.hpp"

struct Arguments;

struct ProcessInfo {
    int pid;
    std::string name;
    std::string status;
    double cpu_usage;
    double in_traffic;
    double out_traffic;
    uint in_bytes;
    uint out_bytes;
    long int used_memory;
    double uptime;

    std::string user;
    std::string group;
    
    long int read_calls;
    long int write_calls;
};

struct NetTraffic{
    uint in;
    uint out;
};

struct IOStat{
    uint in;
    uint out;
};

struct CPUInfo{

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
struct NetTraffic GetSystemNetUsage(Arguments& args);

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


inline double jiffiesToSeconds(long jiffies) {
    return jiffies / sysconf(_SC_CLK_TCK);
}

/**
 * @brief Get the Process Up Time
 * 
 * @param pid The pid of the process
 * @return double The uptime of the process
 */
double getProcessUpTime(int pid);

/**
 * @brief Gets the in/out traffic of a process
 * 
 * @param pid The process identifier
 * @return IOStat An structu with the information
 */
IOStat getIOTraffic(int pid, Arguments& args);


std::vector<long> get_cpu_usage();

double calculate_cpu_percentage(const std::vector<long>& prev, const std::vector<long>& current);

/**
 * @brief Get the process' read calls
 * 
 * @param pid The process identifier
 * @return long int The number of read calls
 */
long int getProcessReadCalls(int pid);

/**
 * @brief Get the process' write calls
 * 
 * @param pid The process identifier
 * @return long int The number of write calls
 */
long int getProcessWriteCalls(int pid);

std::map<int, double> getProcessCpuUsage(std::map<int, double> &prev_cpu_times);

#endif