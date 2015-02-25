#include <iostream>
#include <string>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "util.h"

#include "usage.cpp"

#define CWDS string(".")
#define PWDS string("..")


using namespace std;


int main(int argc, char* argv[]) 
{
    vector <string> paths;
    string destination;

    Util util = Util();

    for (int i = 1; i < argc; i++) {
        string arg = string(argv[i]);

        if (string(argv[i]) == "--findc") {
            if ((i + 1) < argc) {
                util.addQuery(argv[i + 1]);
                i++;
            }
            else {
                
                show_usage("'find' command requires at least one argument.");

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


    util.scan(paths);

    if (util.hitCount() == 0) {
        cout << "Nothing found" << endl;
    }

    return 0;
}

