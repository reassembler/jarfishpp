#include <iostream>
#include <string>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "util.h"

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
                cerr << "'find' command requires at least one argument." << endl;
            }
        }
        else {
            paths.push_back(arg);
        }
    }

    if (!util.hasQuery()) {
        cerr << "At least one query is required." << endl;

        return 1;
        // EARLY OUT
    }

    if (paths.size() == 0) {
        cerr << "At least one path is required." << endl;

        return 1;
        // EARLY OUT
    }

    util.scan(paths);

    return 0;
}

