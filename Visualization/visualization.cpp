#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <fcntl.h>

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


    drawHorizontalBar();

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
                if(args.u_display)
                    cout << (sort_counter==i+1? GREEN: "") << setw(16) << "User" << RESET;
            break;
            case 'g':
                if(args.group_display)
                    cout << (sort_counter==i+1? GREEN: "") << setw(16) << "Group" << RESET;

            break;
        }
    }

    drawHorizontalBar();

    // Display process information
    for (auto& process : processes) {
        cout << setw(8) << process.pid << setw(20) << process.name;//<<setw(20)<<process.uptime;
        
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
            }
        }
        cout << "\n";
    }
}

void displayCPUUsage(struct ProcessInfo& process){
    std::cout << setw(8) << process.status << setw(15) << process.cpu_usage*100;
}

void displayNetUsage(struct ProcessInfo& process){
    std::cout << setw(12) <<process.in_traffic << setw(12) << process.out_traffic;
}

void displayRAMUsage(struct ProcessInfo& process){
    std::cout << setw(16) <<process.used_memory;
}

void displayUser(struct ProcessInfo& process){
    std::cout << setw(16) << process.user;
}

void displayGroup(struct ProcessInfo& process){
    std::cout << setw(16) << process.group;
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

void showPreHeader(const vector<ProcessInfo>& processes, Arguments& args)
{
    struct NetTraffic net_traffic;
    net_traffic = GetSystemNetUsage(args);

    cout << BLUE;
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

void drawHorizontalBar()
{
    cout << "\n--------------------------------------------------------------------------------\n";
}