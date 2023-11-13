#include "utilities.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

void printHelp(){
    std::cout << "El programa acepta los siguientes argumentos:\n"
        "-h, --help: imprime esta ayuda\n"
        "-t, --timeout: tiempo de refresco en milisegundos\n"
        "-b, --displayBar: muestra la barra de progreso\n"
        "\n"
        "-g, --saveToCSV: guarda la información en un fichero CSV\n"
        "-n, --number: modifica el número de procesos mostrados\n"
        "-N, --net: muestra el tráfico de red por proceso\n"

        "-m, --memory: muestra la memoria consumida por el proceso\n"
        "-G, --giga: muestra toda la información relacionada con memoria en gigabytes\n"
        "-M, --mega: muestra toda la información relacionada con memoria en megabytes\n";

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
            case 'n':
                args.n_process = atoi(optarg);
                break;
            case 'N':
                args.argument_vector.push_back(opt);
                break;
            case 'm':
                args.argument_vector.push_back(opt);
                break;
            case 'G':
                args.g_display = true;
                break;
            case 'M':
                args.m_display = true;
                break;
            /*TO BE EXPANDED*/
        }
    }

    if(args.g_display && args.m_display){
        args.g_display = false;
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
        
        for(uint i=0; i<arguments.size(); i++){
            switch(arguments[i]){
            case 'r':
                file << ',' << info.in_traffic << ',' << info.out_traffic;
                break;
            }
        }
        file << "\n";
    }

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

void sortProcesses(std::vector<ProcessInfo> &processes, int &sort_counter,
    std::vector<int> arguments)
{
    if((sort_counter - 1) == -1){
        std::sort(processes.begin(), processes.end(), cpu_comparison);
    }else{
        switch(arguments[sort_counter-1]){
            case 'r':
                std::sort(processes.begin(), processes.end(), net_comparison);
                break;
            case 'm':
                std::sort(processes.begin(), processes.end(), ram_comparison);
                break;
        }
    }
}

bool cpu_comparison(const ProcessInfo& process1, const ProcessInfo& process2){
    if(process1.cpu_usage > process2.cpu_usage)
        return true;
    else
        return false;
    
}

bool net_comparison(const ProcessInfo& process1, const ProcessInfo& process2){
    if(process1.in_traffic > process2.in_traffic)
        return true;
    else
        return false;
}

bool ram_comparison(const ProcessInfo& process1, const ProcessInfo& process2){
    if(process1.used_memory > process2.used_memory)
        return true;
    else
        return false;
}