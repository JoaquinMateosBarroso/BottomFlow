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

void DisplayProcessInfo(vector<ProcessInfo>& processes, unsigned number_of_processes,
    std::vector<int> arguments) {
    // Resize the vector if needed
    if (number_of_processes < processes.size()) {
        processes.resize(number_of_processes);
    }

    // Display header
    cout << left << setw(6) << "PID" << setw(40) << "Name" << setw(8) << "Status" << setw(15) << "CPU Usage (%)";
    for(uint i=0; i<arguments.size(); i++){
        switch(arguments[i]){
            case 'r':
                cout << setw(12) << "InTraffic" << setw(12) << "OutTraffic";
        }
    }
    cout << "\n";

    // Display process information
    for (const auto& process : processes) {
        cout << setw(6) << process.pid << setw(40) << process.name;
        
        displayCPUUsage(process);

        
        for(uint i=0; i<arguments.size(); i++){
            switch(arguments[i]){
                case 'r':
                    displayNetUsage(process);
                    break;
            }
        }
        cout << "\n";
    }
}

void displayCPUUsage(struct ProcessInfo process){
    std::cout << setw(8) << process.status << setw(15) << process.cpu_usage;
}

void displayNetUsage(struct ProcessInfo process){
    std::cout << setw(12) <<process.in_traffic << setw(12) << process.out_traffic;
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
        option::BarWidth{80},
        option::Start{"["},
        option::End{"]"},
        option::ForegroundColor{Color::green}  ,
        option::FontStyles{vector<FontStyle>{FontStyle::bold}}
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