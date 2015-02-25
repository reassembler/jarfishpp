#include <vector>
#include <string>

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "util.h"

#include "miniz.c"



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

bool endsWith(string s, string test)
{
    if (s.length() >= test.length()) {
        if (s.compare(s.length() - test.length(), test.length(), test) == 0) {
            return true;
        }    
    }

    return false;
}

vector<Hit> Util::matchesQuery(string test)
{
    vector<Hit> hits;

    for (vector<string>::iterator it = queries.begin(); it != queries.end(); ++it) {
        string query = *it;
        if (test.find(query) != string::npos) {
            hits.push_back(Hit());

            hits.back().name = test;
        }    
    }
        
    return hits;
}


bool Util::isArchive(string path)
{
    for (vector<string>::iterator it = extensions.begin(); it != extensions.end(); ++it) {
        string ext = *it;

        if (endsWith(path, ext)) {
            return true;
        }
    }

    return false;
}


void Util::searchArchive(string fileName) 
{
    int i, sort_iter;
    mz_bool status;
    size_t uncomp_size;
    mz_zip_archive zip_archive;
    void *p;
    const int N = 50;
    char data[2048];
    char archive_filename[64];

    memset(&zip_archive, 0, sizeof(zip_archive));

    status = mz_zip_reader_init_file(&zip_archive, fileName.c_str(), 0);

    if (status) {
        int fileCount = (int) mz_zip_reader_get_num_files(&zip_archive);

        for (i = 0; i < fileCount; i++) {
            mz_zip_archive_file_stat file_stat;

            if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
                printf("mz_zip_reader_file_stat() failed!\n");
                mz_zip_reader_end(&zip_archive);
            }

            string entryName = string(file_stat.m_filename);

            if (endsWith(entryName, ".class")) {
                vector<Hit> hits = matchesQuery(entryName);

                if (hits.size() > 0) {
                    for (int i = 0; i < hits.size(); i++) {
                        hits[i].context = fileName;

                        this->hits.push_back(hits[i]);

                        cout << hits[i].context << endl;
                        cout << "    " << hits[i].name << endl;
                    }
                }
            }
        }

        mz_zip_reader_end(&zip_archive);
    }
    else {
        cout << "could not open archive: " << fileName << endl;
    }
}

void Util::listDirectory(string path) 
{
    DIR *dp;
    struct dirent *dirp;

    dp = opendir(path.c_str());

    if (dp != NULL) {
        while ((dirp = readdir(dp)) != NULL) {
            if (dirp->d_name != string(".") && dirp->d_name != string("..")) {
                string searchPath = path + kPathSeparator + dirp->d_name;

                if (isDirectory(searchPath) == true) {
                    listDirectory(searchPath);
                }
                else if(isArchive(searchPath) == true) {
                    searchArchive(searchPath);
                }
            }
        }

        closedir(dp);
    }
    else {
        cerr << "Couldn't open dir: '" << path << "' error: " << strerror(errno) << endl;
    }
}


void Util::scan(vector<string> paths) 
{
    cout << "scanning..." << endl;
    for (vector<string>::iterator it = paths.begin(); it != paths.end(); ++it) {
        listDirectory(*it);
    }
}
