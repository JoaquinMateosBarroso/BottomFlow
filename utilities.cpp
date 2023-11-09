#include "utilities.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

void printHelp(){
    std::cout << "El programa acepta los siguientes argumentos:\n"
        "-h --help: imprime esta ayuda\n"
        "-t --timeout: tiempo de refresco en milisegundos\n"
        "-b --displayBar: muestra la barra de progreso\n"
        "\n"
        "-g --saveToCSV: guarda la información en un fichero CSV\n";
}


Arguments parseArgs(int argc, char* argv[]){
    Arguments args;
    int opt = 0;
    while(opt != -1){
        opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        switch(opt){
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
            case 't':
                args.timeout = atoi(optarg);
                break;
            case 'b':
                args.displayBar = true;
                break;
            case 'g':
                args.csvName = optarg;
                break;
            /*TO BE EXPANDED*/
        }
    }
    return args;
}


void SaveToCSV(const vector<ProcessInfo>& data, const string& filename, int timeout)
{
    std::ofstream file(filename, std::ios_base::app); // Open in append mode

    // Get current time
    std::time_t currentTime = std::time(nullptr);

    // Write data to the CSV file
    for (const auto& info : data) {
        // Format timestamp
        std::tm* timeinfo = std::localtime(&currentTime);
        std::ostringstream oss;
        oss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");

        file << oss.str() << ',' << info.pid << ',' << info.name << ',' << info.status << ',' << info.cpu_usage << '\n';
    }

    // Close the file
    file.close();

}


void SaveToCSVHeader(const string &fileName)
{
    std::ofstream file(fileName);
    file << "Time,PID,Name,Status,CPU Usage (%)\n";
    file.close();
}