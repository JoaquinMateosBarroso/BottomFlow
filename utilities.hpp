#ifndef UTILITIES_HPP
#define UTILITIES_HPP
 
#include <getopt.h>
#include <vector>

#include "infoAcquisition.hpp"

/*TO BE EXPANDED*/
const char* const short_opts = "ht:bg:n:r";

const option long_opts[] = {
    {"help", no_argument, nullptr, 'h'},
    {"timeout", required_argument, nullptr, 't'},
    {"displaybar", no_argument, nullptr, 'b'},

    {"saveToCSV", required_argument, nullptr, 'g'},
    {"number", required_argument, nullptr, 'n'},
    {"net", no_argument, nullptr, 'r'}
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
 * @param argv Vector of arugments
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


#endif