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
    vector <string> queries;
    vector <string> paths;
    string destination;

    for (int i = 1; i < argc; i++) {
        string arg = string(argv[i]);

        if (string(argv[i]) == "--find") {
            if ((i + 1) < argc) {
                queries.push_back(argv[i + 1]);
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

    if (queries.size() > 0) {
        cout << "Searching for: " << endl;

        for (vector<string>::iterator it = queries.begin(); it != queries.end(); ++it) {
            cout << "  " << *it << endl;
        }
    }
    else {
        cerr << "At least one query is required." << endl;

        return 1;
        // EARLY OUT
    }

    if (paths.size() == 0) {
        cerr << "At least one path is required." << endl;

        return 1;
        // EARLY OUT
    }

    Util::scan(paths);

    return 0;
}

