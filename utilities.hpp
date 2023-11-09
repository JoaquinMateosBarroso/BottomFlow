#ifndef UTILITIES_HPP
#define UTILITIES_HPP
 
#include <getopt.h>
#include <vector>

#include "infoAcquisition.hpp"

/*TO BE EXPANDED*/
const char* const short_opts = "ht:bg:";

const option long_opts[] = {
    {"help", no_argument, nullptr, 'h'},
    {"timeout", required_argument, nullptr, 't'},
    {"displaybar", no_argument, nullptr, 'b'},

    {"saveToCSV", required_argument, nullptr, 'g'},
};

struct Arguments {
    int timeout = 2000;
    bool displayBar = false;

    std::string csvName = "";
};

/**
 * @brief Prints help for command line parser
 * 
 */
void printHelp();


Arguments parseArgs(int argc, char* argv[]);    



void SaveToCSV(const std::vector<ProcessInfo>& data, const std::string& filename, int timeout);

void SaveToCSVHeader(const std::string &fileName);


#endif