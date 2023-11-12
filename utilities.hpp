#ifndef UTILITIES_HPP
#define UTILITIES_HPP
 
#include <getopt.h>
#include <vector>

#include "infoAcquisition.hpp"

/*TO BE EXPANDED*/
const char* const short_opts = "ht:bg:n:Nm";

const option long_opts[] = {
    {"help", no_argument, nullptr, 'h'},
    {"timeout", required_argument, nullptr, 't'},
    {"displaybar", no_argument, nullptr, 'b'},

    {"saveToCSV", required_argument, nullptr, 'g'},
    {"number", required_argument, nullptr, 'n'},
    {"net", no_argument, nullptr, 'N'},
    {"memory", no_argument, nullptr, 'm'}
};

struct Arguments {
    int timeout = 2000;
    bool displayBar = false;
    int n_process = 10;

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
    std::vector<int> arguments);

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
    std::vector<int> arguments);

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
 * @brief Compares network usage of processes
 * 
 
 */
bool net_comparison(const ProcessInfo& process1, const ProcessInfo& process2);

/**
 * @brief Compares RAM usage of processes
 * 
 * @param process1 The first process
 * @param process2 The second process
 * @return true First process RAM usage is greater
 * @return false First process RAM usage is lower
 */
bool ram_comparison(const ProcessInfo& process1, const ProcessInfo& process2);

#endif