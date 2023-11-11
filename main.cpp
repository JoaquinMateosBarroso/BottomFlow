#include <iostream>
#include <unistd.h>

#include "utilities.hpp"
#include "infoAcquisition.hpp"
#include "visualization.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    int in = 0;

    Arguments args = parseArgs(argc, argv);

    if (args.csvName != "")
        SaveToCSVHeader(args.csvName);

    
    while (in != 'e')
    {
        system("clear");

        vector<ProcessInfo> processes = ReadProcFileSystem();
        DisplayProcessInfo(processes, args.n_process, args.argument_vector);

        if (args.csvName != "")
            SaveToCSV(processes, args.csvName, args.timeout);

        if (in == 's')
            cout << "Aquí tendremos que implementar la lógica para pedir el parámetro por el que ordenamos" << endl;

        if (args.displayBar)
            DisplayBar(args.timeout);
        else
            usleep(args.timeout*1000); // usleep sleeps microseconds, so we need to multiply by 1000 to get milliseconds


        in = getKey(1);
    }
    return 0;
}
