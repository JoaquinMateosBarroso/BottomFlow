#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <iterator>
#include <thread>
#include <fcntl.h>

#include "infoAcquisition.hpp"
#include "visualization.hpp"


using namespace std;






int main() {
    int in = 1;
    in = getKey(500);
    
    while (in != 'e')
    {
        system("clear");
        std::vector<ProcessInfo> processes = ReadProcFileSystem();
        DisplayProcessInfo(processes);
        DisplayBar();
    }
    return 0;
}
