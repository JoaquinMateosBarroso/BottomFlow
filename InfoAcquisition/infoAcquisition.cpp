#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <iterator>
#include <cstring>
#include <map>

#include "infoAcquisition.hpp"

using namespace std;



std::map<int, double> getProcessCpuUsage();



std::vector<ProcessInfo> ReadProcFileSystem(Arguments& args) {
    std::vector<ProcessInfo> processes;

    // Open the "/proc" directory.
    std::string proc_dir = "/proc";
    DIR* dp = opendir(proc_dir.c_str());

    if (!dp) {
        std::cerr << "Failed to open /proc directory." << std::endl;
        exit(1);
    }

    map<int, double> cpuUsageMap = getProcessCpuUsage();

    struct dirent* entry;
    while ((entry = readdir(dp))) {
        
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
            process.cpu_usage = cpuUsageMap[process.pid];

            for(uint i=0; i<args.argument_vector.size(); i++){
                switch(args.argument_vector[i]){
                    case 'm':
                        process.used_memory = getProcessRAMUsage(process.pid, args);
                    break;
                    case 'u':
                        process.user = getProcessUser(process.pid);
                    break;
                    case 'g':
                        process.group = getProcessGroup(process.pid);
                    break;
                    case 'U':
                        process.uptime = getProcessUpTime(process.pid);
                    break;
                    case 'D':
                        struct IOStat stats;
                        stats = getIOTraffic(process.pid, args);
                        process.in_bytes = stats.in;
                        process.out_bytes = stats.out;
                    break;
                    case 'r':
                        process.read_calls = getProcessReadCalls(process.pid);
                    break;
                    case 'w':
                        process.write_calls = getProcessWriteCalls(process.pid);
                    break;
                }
            }
            
            processes.push_back(process);
        }
    }



    closedir(dp);
    return processes;
}


int getNumCPUs() {
    std::ifstream cpuinfoFile("/proc/cpuinfo");
    if (!cpuinfoFile.is_open()) {
        std::cerr << "Error opening /proc/cpuinfo\n";
        return -1;  // Return -1 to indicate an error
    }

    std::string line;
    int numCPUs = 0;

    while (std::getline(cpuinfoFile, line)) {
        if (line.find("processor") == 0) {
            numCPUs++;
        }
    }

    cpuinfoFile.close();

    return numCPUs;
}
std::map<int, double> getProcessCpuUsage() {

    struct ProcessCPUUsage {
        double utime;
        double stime;
    };
    std::map<int, ProcessCPUUsage> cpuUsageMap;
    std::map<int, double> outMap;

    DIR* dp = opendir("/proc");
    struct dirent* entry;
    int pid;
    long prev_total_time=0, post_total_time=0;


    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);
    // Skip the "cpu" string at the beginning
    iss.ignore(4, ' ');
    long value;
    while (iss >> value)
        prev_total_time += value;
    while ((entry = readdir(dp)))
    {
        if (isdigit(entry->d_name[0])) {
            pid = std::stoi(entry->d_name);
        }
        else
            continue;

        std::ifstream statFile("/proc/" + std::to_string(pid) + "/stat");
        if (!statFile.is_open()) {
            std::cerr << "Error opening stat file for process " << pid << std::endl;
            outMap[pid] = -1;
        }
        else
        {
            std::string line;
            std::getline(statFile, line);
            std::istringstream iss(line);

            unsigned long int prev_utime, prev_stime;

           
            int wordCount = 0;
            string word;

            // Iterate through the words until we reach the 14th word
            while (iss >> word && wordCount < 14) {
                if (wordCount == 13) {
                    prev_utime =  stoul(word);

                    if (iss >> word) {
                        prev_stime = stoul(word);
                    }
                }
                wordCount++;
            }



            cpuUsageMap[pid].utime = prev_utime;
            cpuUsageMap[pid].stime = prev_stime;
            // cout << pid<<"-"<< prev_utime << endl;
        }
    }
    // cout << "-------------------------------------------";

    

    usleep(200000);


    file = std::ifstream("/proc/stat");
    std::getline(file, line);
    iss = std::istringstream(line);
    // Skip the "cpu" string at the beginning
    iss.ignore(4, ' ');
    while (iss >> value)
        post_total_time += value;

    for (auto i: cpuUsageMap)
    {
        pid = i.first;
        std::ifstream statFile("/proc/" + std::to_string(pid) + "/stat");
        if (!statFile.is_open()) {
            std::cerr << "Error opening stat file for process " << pid << std::endl;
            return std::map<int,double>();
        }
        std::string line;
        std::getline(statFile, line);
        std::istringstream iss(line);

        // Variables to store values from stat file
        long unsigned prev_utime, prev_stime, post_utime, post_stime;
        long unsigned wordCount = 0;
        string word;

        // Iterate through the words until we reach the 14th word
        while (iss >> word && wordCount < 14) {
            if (wordCount == 13) {
                post_utime =  stoul(word);

                if (iss >> word)
                    post_stime = stoul(word);
            }
            wordCount++;
        }

        prev_utime = i.second.utime;
        prev_stime = i.second.stime;

        // double clockTicksPerSecond = sysconf(_SC_CLK_TCK);
        outMap[pid] = (getNumCPUs()) * 100.0 * double(post_utime+post_stime - prev_utime-prev_stime) / (post_total_time - prev_total_time);
        // if (post_utime > prev_utime)
        //     cout << pid << "-" << post_utime-prev_utime << endl;

    }


    return outMap;
}






struct NetTraffic GetSystemNetUsage(Arguments& args){

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

    if(args.g_display){
        net_traffic.in = net_traffic.in >> 20;
        net_traffic.out = net_traffic.out >> 20;
    }else if(args.m_display){
        net_traffic.in = net_traffic.in >> 10;
        net_traffic.out = net_traffic.out >> 10;
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

std::string getProcessUser(int pid) {
    int Uid;
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

        if(key == "Uid:") {
            token >> Uid;   
            break;        
        }
    }

    statusFile.close();

    // Open /etc/passwd file to search the user and get its name
    std::ifstream passwdFile("/etc/passwd");
    if(!passwdFile.is_open()) {
        std::cerr << "Error: Unable to open /etc/passwd" << std::endl;
        return "error";
    }

    while (std::getline(passwdFile, line)) {
        std::istringstream token(line);
        std::string auxUser;

        // Split the line by ':' and get the username, passwd and UID
        std::getline(token, auxUser, ':');  // Name
       
        std::string auxPasswd;
        std::getline(token, auxPasswd, ':');  // Password

        std::string auxId;
        std::getline(token, auxId, ':');  // UID

        // Convert UID string to integer
        int uid = std::stoi(auxId);

        if(uid == Uid)
        {
            user = auxUser;
            break;   
        }
    }

    passwdFile.close();

    return user;
}

std::string getProcessGroup(int pid) {
    int Gid;
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

        if(key == "Gid:") {
            token >> Gid;   
            break;        
        }
    }

    statusFile.close();

    // Open /etc/group file to search the group and get its name
    std::ifstream groupFile("/etc/group");
    if(!groupFile.is_open()) {
        std::cerr << "Error: Unable to open /etc/group" << std::endl;
        return "error";
    }

    while (std::getline(groupFile, line)) {
        std::istringstream token(line);
        std::string auxGroup;

        // Split the line by ':' and get the username, passwd and UID
        std::getline(token, auxGroup, ':');  // Name
       
        std::string auxPasswd;
        std::getline(token, auxPasswd, ':');  // Password

        std::string auxGid;
        std::getline(token, auxGid, ':');  // UID

        // Convert GID string to integer
        int gid = std::stoi(auxGid);

        if(gid == Gid)
        {
            group = auxGroup;
            break;   
        }
    }

    groupFile.close();

    return group;

}

double getProcessUpTime(int pid) {

    std::ifstream statFile("/proc/" + std::to_string(pid) + "/stat");

    if (!statFile) {
        std::cerr << "Error abriendo el archivo /proc/" << pid << "/stat\n";
        return -1;
    }

    std::string line;
    std::getline(statFile, line);

    statFile.close();

    std::istringstream iss(line);
    std::vector<std::string> tokens(std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>());

    long starttime;                                    

    if (tokens.size() >= 22) {
        starttime = std::stol(tokens[21]);
    }

    std::ifstream uptimeFile("/proc/uptime");

    if (!uptimeFile) {
        std::cerr << "Error abriendo el archivo /proc/uptime\n";
        return 1;
    }

    double uptime;
    uptimeFile >> uptime;

    uptimeFile.close();

    double totalTimeSeconds = starttime / static_cast<double>(sysconf(_SC_CLK_TCK));

    double elapsed = uptime - totalTimeSeconds;

    return elapsed;
}


IOStat getIOTraffic(int pid, Arguments& args){

    IOStat stats;
    stats.in = 1;
    stats.out = 1;

    std::string ruta = "/proc/" + std::to_string(pid) + "/io";

    std::ifstream archivo(ruta.c_str(), std::ios::binary);
    if (!archivo.is_open()) {
        //std::cerr << "Error al abrir el archivo " << ruta << ": " << strerror(errno) << std::endl;
        return stats;
    }   

    std::string linea;
    while (std::getline(archivo, linea)) {
        // Buscar la línea que contiene "read_bytes" y "write_bytes"
        if (linea.find("read_bytes") != std::string::npos) {
            std::istringstream iss(linea);
            std::string key;
            long long value;

            // Extraer el valor de "read_bytes"
            iss >> key >> value;
            stats.in = value;
        } else if (linea.find("write_bytes") != std::string::npos) {
            std::istringstream iss(linea);
            std::string key;
            long long value;

            // Extraer el valor de "write_bytes"
            iss >> key >> value;
            stats.out = value;
        }
    }

    if(args.g_display){
        stats.in = stats.in >> 20;
        stats.out = stats.out >> 20;
    }else if(args.m_display){
        stats.in = stats.in >> 10;
        stats.out = stats.out >> 10;
    }

    if(stats.in == 0)
        stats.in = 1;
    if(stats.out == 0)
        stats.out = 1;

    return stats;
}



std::vector<long> get_cpu_usage() {
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);

    std::istringstream iss(line);
    // Skip the "cpu" string at the beginning
    iss.ignore(4, ' ');

    std::vector<long> cpu_times;
    long value;
    while (iss >> value)
        cpu_times.push_back(value);

    return cpu_times;
}

double calculate_cpu_percentage(const std::vector<long>& prev, const std::vector<long>& current) {
    long prev_idle = prev[3];
    long idle = current[3];

    long prev_non_idle = prev[0] + prev[1] + prev[2] + prev[4] + prev[5] + prev[6];
    long non_idle = current[0] + current[1] + current[2] + current[4] + current[5] + current[6];

    long prev_total = prev_idle + prev_non_idle;
    long total = idle + non_idle;

    long total_diff = total - prev_total;
    long idle_diff = idle - prev_idle;

    double cpu_percentage = 100.0 * (total_diff - idle_diff) / total_diff;

    return cpu_percentage;
}

long int getProcessReadCalls(int pid) {
    long int readCalls = 0;
    
    std::string ioFilePath = "/proc/" + std::to_string(pid) + "/io";
    
    std::ifstream ioFile(ioFilePath, std::ios::binary);

    if(!ioFile.is_open()) {
        //std::cerr << "Error: Unable to open " << ioFilePath << std::endl;
        return readCalls;
    }

    std::string line;

    while (std::getline(ioFile, line)) {
        if(line.find("syscr:") != std::string::npos) {
            std::istringstream iss(line);
            std::string key;
            long int value;
            
            iss >> key >> value;

            readCalls = value;
            
            break;
        }
    }

    ioFile.close();

    return readCalls;
}

long int getProcessWriteCalls(int pid) {
    long int writeCalls = 0;
    
    std::string ioFilePath = "/proc/" + std::to_string(pid) + "/io";
    
    std::ifstream ioFile(ioFilePath, std::ios::binary);

    if(!ioFile.is_open()) {
        //std::cerr << "Error: Unable to open " << ioFilePath << std::endl;
        return writeCalls;
    }

    std::string line;

    while (std::getline(ioFile, line)) {
        if(line.find("syscw:") != std::string::npos) {
            std::istringstream iss(line);
            std::string key;
            long int value;
            
            iss >> key >> value;

            writeCalls = value;
            
            break;
        }
    }


    ioFile.close();

    return writeCalls;
}
