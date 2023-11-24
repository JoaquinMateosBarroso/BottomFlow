#include <iostream>
#include <unistd.h>
#include <iomanip>


#include "utilities.hpp"
#include "infoAcquisition.hpp"
#include "visualization.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    int in = 0;
    int sort_counter = 0;

    Arguments args = parseArgs(argc, argv);

    if (args.csvName != "")
        SaveToCSVHeader(args.csvName, args.argument_vector);

    cout << setprecision(5) << fixed;
    string commandFilter;
    string userFilter;
    string groupFilter;
    
    while (in != 'e')
    {
        vector<ProcessInfo> processes = ReadProcFileSystem(args);
        sortProcesses(processes, sort_counter, args.argument_vector, commandFilter, userFilter, groupFilter);
        DisplayProcessInfo(processes, args, sort_counter);

        if (args.csvName != "")
            SaveToCSV(processes, args.csvName, args.timeout, args.argument_vector, args);

        if (args.displayBar)
            DisplayBar(args.timeout);
        else
            usleep(args.timeout*1000); // usleep sleeps microseconds, so we need to multiply by 1000 to get it from milliseconds


        in = getKey(1);

        switch(in)
        {
            case 0: continue;
            case 'e': exit(0);
            case 's':
                sort_counter = (sort_counter + 1) % (args.argument_vector.size()+1);
                break;
            default:
                cout << endl << "Introduce the filter: ";
                switch (in)
                {
                case 'c': cin >> commandFilter; break;
                case 'u': cin >> userFilter; break;
                case 'g': cin >> groupFilter; break;
                }
    	    in = 0;
        }
    }
    
    return 0;
}
