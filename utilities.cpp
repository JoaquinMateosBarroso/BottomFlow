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
        "-g --saveToCSV: guarda la información en un fichero CSV\n"
        "-n --number: modifica el número de procesos mostrados\n"
        "-r --net: muestra el tráfico de red por proceso";
}


Arguments parseArgs(int argc, char* argv[]){
    Arguments args;
    int opt = 0;

    while(opt != -1){
        opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);


        args.argument_vector.push_back(opt);
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
            case 'n':
                args.n_process = atoi(optarg);
                break;
            case 'r':

                break;
            
            /*TO BE EXPANDED*/
        }
    }
    return args;
}


void SaveToCSV(const vector<ProcessInfo>& data, const string& filename, int timeout,
    std::vector<int> arguments)
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

        file << oss.str() << ',' << info.pid << ',' << info.name << ',' << info.status << ',' << info.cpu_usage;
        //',' << info.in_traffic << ',' << info.out_traffic << '\n';
        for(uint i=0; i<arguments.size(); i++){
            switch(arguments[i]){
            case 'r':
                file << ',' << info.in_traffic << ',' << info.out_traffic;
                break;
            }
        }
        file << "\n";
    }

    // Close the file
    file.close();

}


void SaveToCSVHeader(const string &fileName, std::vector<int> arguments)
{
    std::ofstream file(fileName);
    file << "Time,PID,Name,Status,CPU Usage (%)";
    for(uint i=0; i<arguments.size(); i++){
        switch(arguments[i]){
            case 'r':
                file << "InputBytes,OutputBytes";
                break;
        }
    }
    file << "\n";
    file.close();
}