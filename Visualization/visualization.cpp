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

void DisplayProcessInfo(vector<ProcessInfo>& processes, int number_of_processes) {
    processes.resize(number_of_processes); 


    cout << "PID\tName\tStatus" << endl;
    for (auto process : processes) {
        cout << process.pid << "\t" << process.name << "\t" << process.status << "\t"
                  "CPU usage:" << process.cpu_usage << endl;
    }
}

// Function to get key without waiting
// returns 0 if no key is pressed
char getKey(int timeoutMs)
{
    char ch = 0;
    std::chrono::milliseconds timeout(timeoutMs);

    auto start_time = std::chrono::high_resolution_clock::now();
    bool gotChar = false;

    do {
        if (std::chrono::high_resolution_clock::now() - start_time >= timeout) {
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
    using namespace indicators;

    BlockProgressBar bar{
        option::BarWidth{80},
        option::Start{"["},
        option::End{"]"},
        option::ForegroundColor{Color::white}  ,
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
    };

    // Update bar state
    while (true) {
        bar.tick();
        if (bar.is_completed())
        break;
        std::this_thread::sleep_for(std::chrono::milliseconds(int(float(msToCharge)/100)));
    }

    return 0;
}