#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <iterator>
#include <pwd.h> // For getpwuid function
#include <grp.h> // For getgrpid function
#include <cstring>

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

    /*********** Change this to the header **********/
    struct NetTraffic net_traffic;
    net_traffic = GetSystemNetUsage();
    /*********************/

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
                    case 'm':
                        process.used_memory = getProcessRAMUsage(process.pid, args);
                    break;
                    case 'u':
                        process.name = getProcessUser(process.pid);
                    break;
                    case 'g':
                        process.group = getProcessGroup(process.pid);
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

struct NetTraffic GetSystemNetUsage(){

    struct NetTraffic net_traffic;
    net_traffic.in = 0;
    net_traffic.out = 0;

    std::string proc_dir = "/proc";
    std::ifstream net_file(proc_dir + "/net/dev");

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

                unsigned int rx_bytes, tx_bytes;
                iss >> rx_bytes >> tx_bytes;

                net_traffic.in += rx_bytes;
                net_traffic.out += tx_bytes;
            }
        }
        net_file.close();

    }

    net_traffic.in = net_traffic.in >> 10;
    net_traffic.in = net_traffic.in >> 10;
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

std::string getProcessUser(int pid) {
    uid_t Uid;
    std::string user;

    // Open the status file of the process
    std::string statusFilePath = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream statusFile(statusFilePath);

    if(!statusFile.is_open()) {
        std::cerr << "Error: Unable to open " << statusFilePath << std::endl;
        return "error";
    }

    std::string line;

    // Obtain the UID
    while (std::getline(statusFile, line)) {
        std::istringstream token(line);
        std::string key, value;
        token >> key;

        if(key == "Uid") {
            token >> Uid;   
            break;        
        }
    }

    statusFile.close();

    // Obtain the user of the process
    struct passwd *pwd = getpwuid(Uid);

    if(pwd != nullptr)
        user = pwd->pw_name;
    else
        user = "Unkown";

    return user;
}

std::string getProcessGroup(int pid) {
    uid_t Gid;
    std::string group;

    // Open the status file of the process
    std::string statusFilePath = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream statusFile(statusFilePath);

    if(!statusFile.is_open()) {
        std::cerr << "Error: Unable to open " << statusFilePath << std::endl;
        return "error";
    }

    std::string line;

    // Obtain the UID
    while (std::getline(statusFile, line)) {
        std::istringstream token(line);
        std::string key, value;
        token >> key;

        if(key == "Gid") {
            token >> Gid;   
            break;        
        }
    }

    statusFile.close();

    // Obtain the user of the process
    struct group *grp = getgrgid(Gid);

    if(grp != nullptr)
        group = grp->gr_name;
    else
        group = "Unkown";

    return group;

}

