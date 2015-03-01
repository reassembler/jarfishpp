#include <iostream>
#include <string>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#include "util.hpp"

#include "usage.cpp"

#define CWDS string(".")
#define PWDS string("..")


using namespace std;


int main(int argc, char* argv[]) 
{
    vector <string> paths;
    string destination;

    bool showStats = false;

    Util util = Util();

    for (int i = 1; i < argc; i++) {
        string arg = string(argv[i]);

        if (string(argv[i]) == "--internal") {
            util.setSearchInternal(true);
        }
        else if (string(argv[i]) == "--showStats") {
            showStats = true;
        }
        else if (string(argv[i]) == "--finds") {
            util.setSearchStrings(true);

            if ((i + 1) < argc) {
                util.addQuery(argv[i + 1]);
                i++;
            }
            else {
                show_usage("'finds' command requires at least one argument.");

                return 1;

                // EARLY OUT
            }

        }
        else if (string(argv[i]) == "--findc") {
            if ((i + 1) < argc) {
                util.addQuery(argv[i + 1]);
                i++;
            }
            else {
                
                show_usage("'findc' command requires at least one argument.");

                return 1;

                // EARLY OUT
            }
        }
        else {
            paths.push_back(arg);
        }
    }

    if (!util.hasQuery()) {
        show_usage("At least one query is required.");

        return 1;
        
        // EARLY OUT
    }

    if (paths.size() == 0) {
        show_usage("At least one path is required.");

        return 1;
        // EARLY OUT
    }


    clock_t start = clock();

    util.scan(paths);

    double elapsed = clock() - start;
    elapsed = elapsed / CLOCKS_PER_SEC * 1000;

    if (showStats) {
        cout << "archives scanned: " << util.archivesScanned << endl;
        cout << "archive files tested: " << util.entriesTested << endl;
        cout << "Total elapsed time: " << elapsed << "ms" << endl;
    }

    if (util.hitCount() == 0) {
        cout << "Nothing found" << endl;
    }

    return 0;
}

