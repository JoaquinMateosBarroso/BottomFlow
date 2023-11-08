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
#include <stdlib.h>

#include "infoAcquisition.hpp"
#include "visualization.hpp"


using namespace std;


/*TO BE EXPANDED*/
const char* const short_opts = "h";
const option long_opts[] = {
    {"help", no_argument, nullptr, 'h'}
};

void printHelp(){
    std::cout << "El programa acepta los siguientes argumentos:\n"
        "-h --help: imprime esta ayuda";
    /*TO BE EXPANDED*/
}

int main() {
    bool still_reading = true;
    int in = 1;
    in = getKey(500);

    while(still_reading){
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        
        if(opt == -1){
            still_reading = false;
        }

        switch(opt){
            case 'h':
            printHelp();
            exit(EXIT_SUCCESS);
            break;
            /*TO BE EXPANDED*/
        }
    }
    
    while (in != 'e')
    {
        system("clear");
        std::vector<ProcessInfo> processes = ReadProcFileSystem();
        DisplayProcessInfo(processes);
        DisplayBar();
    }
    return 0;
}
