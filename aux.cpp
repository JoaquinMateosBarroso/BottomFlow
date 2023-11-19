#include <iostream>
#include <cstdio>
#include <cstring>
#include <map>

std::map<int, double> getProcessCpuUsage() {
    std::map<int, double> cpuUsageMap;

    FILE* fp = popen("ps -e -o pid,%cpu", "r");

    if (fp == nullptr) {
        std::cerr << "Error opening pipe to ps command." << std::endl;
        return cpuUsageMap; // Return an empty map in case of an error
    }

    // Read the header line
    char buffer[128];
    if (fgets(buffer, sizeof(buffer), fp) == nullptr) {
        std::cerr << "Error reading ps output." << std::endl;
        pclose(fp);
        return cpuUsageMap; // Return an empty map in case of an error
    }

    // Read process information and store in the map
    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        int pid;
        double cpuUsage;

        // Parse the PID and CPU usage from the output
        if (sscanf(buffer, "%d %lf", &pid, &cpuUsage) == 2) {
            cpuUsageMap[pid] = cpuUsage;
        } else {
            std::cerr << "Error parsing ps output." << std::endl;
        }
    }

    pclose(fp);
    return cpuUsageMap;
}

int main() {
    // Call the function to get process CPU usage
    std::map<int, double> cpuUsageMap = getProcessCpuUsage();

    // Print the map
    for (const auto& entry : cpuUsageMap) {
        std::cout << "PID: " << entry.first << ", CPU Usage: " << entry.second << "%" << std::endl;
    }

    return 0;
}
