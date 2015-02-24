#include <vector>
#include <string>

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "zlib.h"

#include "util.h"



#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif


#define CHUNK 16384


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


bool Util::isArchive(string path)
{
    for (vector<string>::iterator it = extensions.begin(); it != extensions.end(); ++it) {
        string ext = *it;
        if (path.length() >= ext.length()) {
            if (path.compare(path.length() - ext.length(), ext.length(), ext)
                    == 0) {
                return true;
            }    
        }
    }
    

    return false;
}

void dumpArchive(string fileName) 
{
    cout << "dumping: " << fileName << endl;

    FILE *file = fopen(fileName.c_str(), "rBR");

    if (file) {
        cout << " file open" << endl;

        int ret, flush;
        unsigned have;
        z_stream strm;
        unsigned char in[CHUNK];
        unsigned char out[CHUNK];

        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;

        ret = inflateInit(&strm);

        if (ret != Z_OK) {
            cout << "NOT OK" << endl;
        }
        else {
            cout << "ITS OK" << endl;
        }


        fclose(file);
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
                else if(isArchive(searchPath) == true) {
                    dumpArchive(searchPath);
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
