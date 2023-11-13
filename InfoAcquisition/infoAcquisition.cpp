#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <iterator>


#include "infoAcquisition.hpp"

std::vector<ProcessInfo> ReadProcFileSystem(Arguments& args) {
    std::vector<ProcessInfo> processes;

    // Open the "/proc" directory.
    std::string proc_dir = "/proc";
    DIR* dp = opendir(proc_dir.c_str());

    if (!dp) {
        std::cerr << "Failed to open /proc directory." << std::endl;
        exit(1);
    }

    struct dirent* entry;
    while ((entry = readdir(dp))) {
        // Check if the entry is a directory and represents a process (numeric name).
        if (isdigit(entry->d_name[0])) {
            ProcessInfo process;
            process.pid = std::stoi(entry->d_name);

            // Read the process status file.
            std::string status_file_path = proc_dir + "/" + entry->d_name + "/status";
            std::ifstream status_file(status_file_path);

            if (status_file.is_open()) {
                std::string line;
                while (std::getline(status_file, line)) {
                    std::istringstream iss(line);
                    std::string key, value;
                    iss >> key >> value;

                    if (key == "Name:") {
                        process.name = value;
                    } else if (key == "State:") {
                        process.status = value;
                    }

                    // Add more fields if needed.
                }
                status_file.close();
            }
            process.cpu_usage = GetProcessCpuUsage(process.pid);

            for(uint i=0; i<args.argument_vector.size(); i++){
                switch(args.argument_vector[i]){
                    case 'r':
                        struct NetTraffic net_traffic;
                        net_traffic = GetProcessNetUsage(process.pid);
                        process.in_traffic = net_traffic.in;
                        process.out_traffic = net_traffic.out;
                    break;
                    case 'm':
                        process.used_memory = getProcessRAMUsage(process.pid, args);
                    break;
                }
            }
            
            processes.push_back(process);
        }
    }

    closedir(dp);
    return processes;
}



double GetProcessCpuUsage(int pid) {
    // Open the "/proc" directory for the process.
    std::string proc_dir = "/proc/" + std::to_string(pid);
    std::ifstream stat_file(proc_dir + "/stat");

    if (stat_file.is_open()) {
        std::string line;
        std::getline(stat_file, line);
        std::istringstream iss(line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                       std::istream_iterator<std::string>{}};

        // Extract the required information from the stat file.
        if (tokens.size() >= 22) {
            long utime = std::stol(tokens[13]);
            long stime = std::stol(tokens[14]);
            long total_time = utime + stime;
            double seconds = sysconf(_SC_CLK_TCK);  // Number of clock ticks per second

            // Calculate CPU usage as a percentage.
            double cpu_usage = (total_time / seconds) / GetTotalCpuTime() * 100;

            return cpu_usage;
        }
    }

    return 0.0; // Process not found or data unavailable.
}


double GetTotalCpuTime() {
    std::ifstream stat_file("/proc/stat");

    if (stat_file.is_open()) {
        std::string line;
        std::getline(stat_file, line);
        std::istringstream iss(line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                       std::istream_iterator<std::string>{}};

        if (tokens.size() > 1) {
            long total_time = 0;
            for (size_t i = 1; i < tokens.size(); i++) {
                total_time += std::stol(tokens[i]);
            }
            return static_cast<double>(total_time);
        }
    }

    return 1.0; // Return a small value to avoid division by zero.
}

struct NetTraffic GetProcessNetUsage(int pid){
    std::string proc_dir = "/proc/" + std::to_string(pid);
    std::ifstream net_file(proc_dir + "/net/dev");

    struct NetTraffic net_traffic;
    net_traffic.in = -1;
    net_traffic.out = -1;

    if(net_file.is_open()){
        std::string line;
        std::getline(net_file, line);
        std::getline(net_file, line);

        while (std::getline(net_file, line)) {
            std::istringstream iss(line);
            std::string interface;
            iss >> interface;

            //Interfaces filtered by :
            if (interface.back() == ':') {
                //':' deletion
                interface.pop_back();

                //Read stats
                unsigned long long int rx_bytes, tx_bytes;
                iss >> rx_bytes >> tx_bytes;

                //Imprimir estadísticas de red para la interfaz
                net_traffic.in = rx_bytes;
                net_traffic.out = tx_bytes;
            }
        }
        net_file.close();
    }
    return net_traffic;
}


long int getProcessRAMUsage(int pid, Arguments& args) {
    long int rss = -1;  

    std::string statusFilePath = "/proc/" + std::to_string(pid) + "/status";

    std::ifstream statusFile(statusFilePath);
    if (!statusFile.is_open()) {
        std::cerr << "Error: Unable to open " << statusFilePath << std::endl;
        return rss;
    }

    std::string line;
    while (std::getline(statusFile, line)) {
        std::istringstream iss(line);
        std::string key, value;
        iss >> key;

        if (key == "VmRSS:") {
            iss >> rss;
            break;
        }
    }

    statusFile.close();
    
    if(args.g_display)
        rss = rss >> 20;
    else if(args.m_display)
        rss = rss >> 10;

    return rss;
}



