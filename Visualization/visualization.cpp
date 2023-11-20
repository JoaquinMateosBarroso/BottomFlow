#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <fcntl.h>
#include <iomanip>


#include "visualization.hpp"
#include <indicators.hpp>

using namespace std;

// ANSI escape codes for text color
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[1;34m"


void DisplayProcessInfo(vector<ProcessInfo>& processes, Arguments& args, int sort_counter) {

    showPreHeader(processes, args);

    // Resize the vector if needed
    if ((uint)args.n_process < processes.size()) {
        processes.resize(args.n_process);
    }

    drawHorizontalBar(args.argument_vector);

    // Display header
    cout << left << setw(8) << "PID" << setw(20) << "Command" << setw(8) << "Status" << RESET;
    cout << (sort_counter==0? GREEN: "") << setw(15) << "CPU Usage (%)" << RESET;
    
    int size = args.argument_vector.size();

    for(int i=0; i<size; i++){
        switch(args.argument_vector[i]){
            case 'N':
                cout << (sort_counter==i+1? GREEN: "") << setw(12) << "InTraffic" << RESET;
                cout << (sort_counter==i+1? GREEN: "") << setw(12) << "OutTraffic" << RESET;
            break;
            case 'm':
                if(args.g_display)
                    cout << (sort_counter==i+1? GREEN: "") << setw(16) << "UsedMemory(GB)" << RESET;
                else if(args.m_display)
                    cout << (sort_counter==i+1? GREEN: "") << setw(16) << "UsedMemory(MB)" << RESET;
                else
                    cout << (sort_counter==i+1? GREEN: "") << setw(16) << "UsedMemory(KB)" << RESET;
            break;
            case 'u':
                cout << (sort_counter==i+1? GREEN: "") << setw(10) << "User" << RESET;
            break;
            case 'g':
                cout << (sort_counter==i+1? GREEN: "") << setw(10) << "Group" << RESET;
            break;
            case 'U':
                cout << (sort_counter==i+1? GREEN: "") << setw(16) << "Uptime" << RESET;
            break;
            case 'D':
                if(args.g_display)
                    cout << (sort_counter==i+1? GREEN: "") << setw(16) << "ReadDisk(GB)" <<
                        setw(16) << "WriteDisk(GB)" << RESET;
                else if(args.m_display)
                    cout << (sort_counter==i+1? GREEN: "") << setw(16) << "ReadDisk(MB)" << 
                        setw(16) << "WriteDisk(MB)" << RESET;
                else
                    cout << (sort_counter==i+1? GREEN: "") << setw(16) << "ReadDisk(KB)" <<
                        setw(16) << "WriteDisk(KB)" << RESET;
            break;
            case 'r':
                cout << (sort_counter==i+1? GREEN: "") << setw(16) << "ReadCalls" << RESET;
            break;
            case 'w':
                cout << (sort_counter==i+1? GREEN: "") << setw(16) << "WriteCalls" << RESET;
            break;
                
        }
    }

    drawHorizontalBar(args.argument_vector);

    // Display process information
    for (auto& process : processes) {
        cout << setw(8) << process.pid << setw(20) << process.name;
        
        displayCPUUsage(process);

        for(uint i=0; i<args.argument_vector.size(); i++){
            switch(args.argument_vector[i]){
                case 'N':
                    displayNetUsage(process);
                    break;
                case 'm':
                    displayRAMUsage(process);
                    break;
                case 'u':
                    displayUser(process);
                    break;
                case 'g':
                    displayGroup(process);
                    break;
                case 'U':
                    displayUptime(process);
                    break;
                case 'D':
                    displayIOStats(process);
                    break;
                case 'r':
                    displayReadCalls(process);
                    break;
                case 'w':
                    displayWriteCalls(process);
                    break;
            }
        }
        cout << "\n";
    }
}

void displayCPUUsage(struct ProcessInfo& process){
    std::cout << std::fixed << std::setprecision(2);
    std::cout << setw(8) << process.status << setw(15) << process.cpu_usage;
}

void displayNetUsage(struct ProcessInfo& process){
    std::cout << setw(12) << process.in_traffic << setw(12) << process.out_traffic;
}

void displayRAMUsage(struct ProcessInfo& process){
    std::cout << setw(16) <<process.used_memory;
}

void displayUser(struct ProcessInfo& process){
    std::cout << setw(10) << process.user;
}

void displayGroup(struct ProcessInfo& process){
    std::cout << setw(10) << process.group;
}

void displayUptime(struct ProcessInfo& process){
    int minutes = process.uptime/60;
    int seconds = (int)process.uptime%60;
    std::string display = std::to_string(minutes) + "min " + std::to_string(seconds) + "sec";

    std::cout << setw(16) << display;
}

void displayIOStats(struct ProcessInfo& process){
    std::cout << setw(16) << process.in_bytes << setw(16) << process.out_bytes;
}

void displayReadCalls(struct ProcessInfo& process){
    std::cout << setw(16) << process.read_calls;
}

void displayWriteCalls(struct ProcessInfo& process){
    std::cout << setw(16) << process.write_calls;
}

char getKey(int timeoutMs)
{
    char ch = 0;
    chrono::milliseconds timeout(timeoutMs);

    auto start_time = chrono::high_resolution_clock::now();
    bool gotChar = false;

    do {
        if (chrono::high_resolution_clock::now() - start_time >= timeout) {
            break; // Timeout reached
        }

        struct termios oldt, newt;
        int oldf, newf;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);

        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        newf = oldf | O_NONBLOCK;
        fcntl(STDIN_FILENO, F_SETFL, newf);

        int key = getchar();
        if (key != EOF) {
            ch = static_cast<char>(key);
            gotChar = true;
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
    } while (!gotChar);

    return ch;
}


int DisplayBar(int msToCharge)
{
    cout << endl;
    
    using namespace indicators;

    BlockProgressBar bar{
        indicators::option::BarWidth{80},
        indicators::option::Start{"["},
        indicators::option::End{"]"},
        indicators::option::ForegroundColor{Color::green}  ,
        indicators::option::FontStyles{vector<FontStyle>{FontStyle::bold}}
    };

    // Update bar state
    while (true) {
        bar.tick();
        // Alternatively, you may use `set_progress` to update the progress bar
        // bar.set_progress(10*i++);

        if (bar.is_completed())
        break;
        this_thread::sleep_for(chrono::milliseconds(int(float(msToCharge)/100)));
    }

    return 0;
}

int countProcesses(const vector<ProcessInfo>& processes, const string& status)
{
    int counter = 0;
    for (auto& process : processes) {
        if (process.status == status)
            counter++;
    }
    return counter;
}

void display_cpu_bars(double cpu_percentage) {
    int num_bars = 50;
    int num_filled_bars = static_cast<int>(cpu_percentage / 2.0);

    std::cout << "[CPU Usage]: [";
    for (int i = 0; i < num_bars; ++i) {
        if (i < num_filled_bars) {
            std::cout << "=";
        } else {
            std::cout << " ";
        }
    }
    std::cout << "] " << cpu_percentage << "%" << std::endl;
}

void showPreHeader(const vector<ProcessInfo>& processes, Arguments& args)
{
    cout << BLUE;
    struct NetTraffic net_traffic;
    net_traffic = GetSystemNetUsage(args);
    
    double cpu_percentage = -1;
    if (args.prev_cpu_times.empty())
        args.prev_cpu_times = get_cpu_usage();
    else
    {
        std::vector<long> current_cpu_times = get_cpu_usage();
        cpu_percentage = calculate_cpu_percentage(args.prev_cpu_times, current_cpu_times);
        args.prev_cpu_times = current_cpu_times;

        display_cpu_bars(cpu_percentage);
    }

    cout << "[PROCESS]:  Total number->" << processes.size() << 
            ", Running->" << countProcesses(processes, "R") <<
            ", Sleeping->" << countProcesses(processes, "S") <<
            ", Stopped->" << countProcesses(processes, "T") <<
            ", Zombie->" << countProcesses(processes, "Z") << 
            "\n";
    cout << RESET;

    std::string unit;
    if(args.g_display)
        unit = "(GB)";
    else if(args.m_display)
        unit = "(MB)";
    else
        unit = "(B)";
    
    cout << BLUE;
    cout << "[NET]: Received" << unit <<": " << net_traffic.in <<
    "    Sent" << unit << ": " << net_traffic.out;
    cout << RESET;
}

void drawHorizontalBar(std::vector<int> args)
{
    cout << "\n---------------------------------------------------";
    int size = args.size();
    for(int i=0; i<size; i++){
        switch(args[i]){
            case 'N':
                cout << "------------";
            break;
            case 'm':
                cout << "----------------";
            break;
            case 'u':
                cout << "----------";
            break;
            case 'g':
                cout << "----------";
            break;
            case 'U':
                cout << "----------------";
            break;
            case 'D':
                cout << "----------------";
            break;
            case 'r':
                cout << "----------------";
            break;
            case 'w':
                cout << "----------------";
            break;

        }
    }
    cout << "\n";
}
