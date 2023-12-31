#ifndef UTILITIES_HPP
#define UTILITIES_HPP
 
#include <getopt.h>
#include <vector>

#include "infoAcquisition.hpp"

struct ProcessInfo;

/*TO BE EXPANDED*/
const char* const short_opts = "ht:bc:n:NmugrwGMUD";

const option long_opts[] = {
    {"help", no_argument, nullptr, 'h'},
    {"timeout", required_argument, nullptr, 't'},
    {"displaybar", no_argument, nullptr, 'b'},
    {"saveToCSV", required_argument, nullptr, 'c'},
    {"number", required_argument, nullptr, 'n'},
    {"net", no_argument, nullptr, 'N'},
    {"memory", no_argument, nullptr, 'm'},
    {"read", no_argument, nullptr, 'r'},
    {"write", no_argument, nullptr, 'w'},
    {"user", no_argument, nullptr, 'u'},
    {"group", no_argument, nullptr, 'g'},
    {"giga", no_argument, nullptr, 'G'},
    {"mega", no_argument, nullptr, 'M'},
    {"uptime", no_argument, nullptr, 'U'},
    {"disk", no_argument, nullptr, 'D'}
};

struct Arguments {
    int timeout = 2000;
    bool displayBar = false;
    int n_process = 10;
    bool g_display = false;
    bool m_display = false;
    bool read_display = false;
    bool write_display = false;
    bool u_display = false;
    bool group_display = false;

    
    std::vector<long> prev_cpu_times;
    double cpu_percentage = -1.0;

    std::map<int, double> prev_proccess_times;
    long prev_total_time = 0;

    std::string csvName = "";
    std::vector<int> argument_vector;
};

/**
 * @brief Prints help for command line parser
 * 
 */
void printHelp();


/**
 * @brief Parses the arguments from command line
 * 
 * @param argc Number of arguments
 * @param argv Vector of arguments
 * @return Arguments The arguments properly treated
 */
Arguments parseArgs(int argc, char* argv[]);    


/**
 * @brief Saves information to a CSV file
 * 
 * @param data The data to be stored
 * @param filename The name of the file
 * @param timeout The frequency of updating
 * @param arguments The elements to be stored
 */
void SaveToCSV(const std::vector<ProcessInfo>& data, const std::string& filename, int timeout,
    std::vector<int> arguments, const Arguments &args);

/**
 * @brief Saves the header of the CSV file
 * 
 * @param fileName The name of the file
 * @param arguments The elements to be displayed
 */
void SaveToCSVHeader(const std::string &fileName, std::vector<int> arguments);

/**
 * @brief Sorts the processes given a parameter
 * 
 * @param processes The vector of processes
 * @param sort_counter The iterator that will decide the parameter on which the vector
 * will be sorted
 * @param arguments A vector of arguments to retrieve the parameter to sort
 */
void sortProcesses(std::vector<ProcessInfo> &processes, int &sort_counter,
    std::vector<int> arguments, const std::string &commandFilter, 
    const std::string &userFilter, const std::string &groupFilter);

/**
 * @brief Compares CPU usage of processes
 * 
 * @param process1 The first process
 * @param process2 The second process
 * @return true First process CPU usage is greater
 * @return false First process CPU usage is lower
 */
bool cpu_comparison(const ProcessInfo& process1, const ProcessInfo& process2);


/**
 * @brief Compares username of processes
 * 
 * @param process1 The first process
 * @param process2 The second process
 * @return true First process RAM usage is greater
 * @return false First process RAM usage is lower
 */
bool user_comparison(const ProcessInfo& process1, const ProcessInfo& process2);


/**
 * @brief Compares group name of processes
 * 
 * @param process1 The first process
 * @param process2 The second process
 * @return true First process RAM usage is greater
 * @return false First process RAM usage is lower
 */
bool group_comparison(const ProcessInfo& process1, const ProcessInfo& process2);

/**
 * @brief Compares RAM usage of processes
 * 
 * @param process1 The first process
 * @param process2 The second process
 * @return true First process RAM usage is greater
 * @return false First process RAM usage is lower
 */
bool ram_comparison(const ProcessInfo& process1, const ProcessInfo& process2);

/**
 * @brief Compares uptime of processes
 * 
 * @param process1 The first process
 * @param process2 The second process
 * @return true First process uptime is greater
 * @return false First process uptime is lower
 */
bool uptime_comparison(const ProcessInfo& process1, const ProcessInfo& process2);

/**
 * @brief Compares secondary memory usage of a process
 * 
* @param process1 The first process
 * @param process2 The second process
 * @return true First process IO traffic is greater
 * @return false First process IO traffic is lower
 */
bool IO_comparison(const ProcessInfo& process1, const ProcessInfo& process2);

/**
 * @brief Compares number of read calls of a process
 * 
 * @param process1 The first process
 * @param process2 The second process
 * @return true First process read calls is greater
 * @return false First process read calls is lower
 */
bool read_comparison(const ProcessInfo& process1, const ProcessInfo& process2);

/**
 * @brief Compares number of write calls of a process
 * 
 * @param process1 The first process
 * @param process2 The second process
 * @return true First process write calls is greater
 * @return false First process write calls is lower
 */
bool write_comparison(const ProcessInfo& process1, const ProcessInfo& process2);

#endif