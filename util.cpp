#include <vector>
#include <string>

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "util.h"

using namespace std;

bool Util::isDirectory(string path) 
{
    struct stat fileInfo;

    stat(path.c_str(), &fileInfo);

    if (S_ISDIR(fileInfo.st_mode)) {
        return true;
    }
    else {
        return false;
    }
}


void Util::listDirectory(string path) 
{
    DIR *dp;
    struct dirent *dirp;

    cout << "  '" << path << "'" << endl;

    dp = opendir(path.c_str());

    if (dp != NULL) {
        while ((dirp = readdir(dp)) != NULL) {
            if (dirp->d_name != string(".") && dirp->d_name != string("..")) {
                string searchPath = path + kPathSeparator + dirp->d_name;

                if (isDirectory(searchPath) == true) {
                    listDirectory(searchPath);
                }
            }
        }
    }
    else {
        cout << "Couldn't open dir: '" << path << "' error: " << errno << endl;
    }
}


void Util::scan(vector<string> paths) 
{
    cout << "scanning..." << endl;
    for (vector<string>::iterator it = paths.begin(); it != paths.end(); ++it) {

        listDirectory(*it);
    }
}
