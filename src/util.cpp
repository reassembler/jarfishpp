#include <vector>
#include <string>
#include <sstream>

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "util.hpp"

#include "classparser.hpp"

#include "miniz.c"


#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif


#define CHUNK 16384

//#define TIMER

#ifdef TIMER
double totalLoadParse = 0;
#endif

bool Util::isDirectory(std::string path) 
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

bool endsWith(std::string s, std::string test)
{
    if (s.length() >= test.length()) {
        if (s.compare(s.length() - test.length(), test.length(), test) == 0) {
            return true;
        }    
    }

    return false;
}

std::string normalizeClassName(std::string fileName) {
    std::string str = "";

    for(int i = 0; i < fileName.size(); i++) {
        const char ch = fileName[i];

        if (ch == '/') {
            str.push_back('.');
        }
        else {
            str.push_back(ch);
        }
    }

    return str;
}

std::vector<Hit> findMatches(std::vector<std::string> queries, std::vector<std::string> tests) {
    std::vector<Hit> hits;

    for (std::vector<std::string>::iterator it = queries.begin(); it != queries.end(); ++it) {
        std::string query = *it;

        for (std::vector<std::string>::iterator sit = tests.begin(); sit != tests.end(); ++sit) {
            std::string test = *sit;

            if (test.find(query) != std::string::npos) {
                hits.push_back(Hit());

                hits.back().name = test;
            }    
        }
    }

    return hits;
}

std::vector<Hit> Util::matchesQuery(std::string test)
{
    std::vector<Hit> hits;

    std::string normalName = normalizeClassName(test);

    bool checkNormalized = normalName != test;

    for (std::vector<std::string>::iterator it = queries.begin(); it != queries.end(); ++it) {
        std::string query = *it;
        if (test.find(query) != std::string::npos
                || (checkNormalized && normalName.find(query) != std::string::npos)) {
            hits.push_back(Hit());

            hits.back().name = test;
        }    
    }
        
    return hits;
}


bool Util::isArchive(std::string path)
{
    for (std::vector<std::string>::iterator it = extensions.begin(); it != extensions.end(); ++it) {
        std::string ext = *it;

        if (endsWith(path, ext)) {
            return true;
        }
    }

    return false;
}

std::vector<Hit> Util::searchClass(std::istream& in) 
{
    std::vector<Hit> hits;

    ClassParser parser = ClassParser();

    if (parser.testSig(in)) {
        parser.parseAll(in);

        if (searchStrings) {
            std::vector<std::string> poolStrings = parser.getPoolStrings();
            
            hits = findMatches(this->queries, poolStrings);
        }
        else {
            // search for the internal class name
            hits = matchesQuery(parser.className);
        }
    }
    else {
        std::cout << "sig failed" << std::endl;
    }

    return hits;
}


void Util::searchArchive(std::string fileName) 
{
#ifdef DEBUGU
    std::cout << fileName << std::endl;
#endif

    archivesScanned++;

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

        for (int i = 0; i < fileCount; i++) {
            bool emitFileName = true;
            entriesTested++;

            mz_zip_archive_file_stat file_stat;

            if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
                printf("mz_zip_reader_file_stat() failed!\n");

                 // something went wrong while reading the file, just leave
                 break;
            }

            std::string entryName = std::string(file_stat.m_filename);

            if (endsWith(entryName, ".class")) {
#ifdef DEBUGU
                std::cout << entryName << std::endl;
#endif

                std::vector<Hit> hits;
                std::string context = fileName;

                if (searchStrings || searchInternal) {
                    context.append(" -> ");
                    context.append(entryName);
                    
                    // load class file into memory for parsing
                    size_t uncompressed_size = file_stat.m_uncomp_size;
                    void* p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, &uncompressed_size, 0);
                    if (!p) {
                        std::cerr << "mz_zip_reader_extract_file_to_heap() failed..." << std::endl;
                        mz_zip_reader_end(&zip_archive);
                        return;

                        // EARLY OUT
                    }

                    std::string str_unzip;
                    str_unzip.assign((const char*) p,uncompressed_size);

                    stringstream iss(str_unzip);

                    hits = searchClass(iss);

                    mz_free(p);
                }
                else {
                    // just search the class file name
                    hits = matchesQuery(entryName);
                }

                // deal with any hits we made
                if (hits.size() > 0) {
                    int n = hits.size();

                    bool emitContext = true;

                    for (int i = 0; i < n; i++) {
                        hits[i].context = context;

                        if (emitContext) {
                            // only show the file name once per file
                            std::cout << hits[i].context << std::endl;

                            emitContext = false;
                        }

                        std::cout << "    " << hits[i].name << std::endl;
                    }
                }
            }
        }

        mz_zip_reader_end(&zip_archive);
    }
    else {
        std::cerr << "could not open archive: " << fileName << std::endl;
    }
}

void Util::listDirectory(std::string path) 
{
    struct dirent *dirp;

    DIR *dp = opendir(path.c_str());

    if (dp != NULL) {
        while ((dirp = readdir(dp)) != NULL) {
            if (dirp->d_name != std::string(".") && dirp->d_name != std::string("..")) {
                std::string searchPath = path + kPathSeparator + dirp->d_name;

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
        cerr << "Couldn't open dir: '" << path << "' error: " << strerror(errno) << std::endl;
    }
}


void Util::scan(std::vector<std::string> paths) 
{
    std::cout << "scanning..." << std::endl;
    for (std::vector<std::string>::iterator it = paths.begin(); it != paths.end(); ++it) {
        listDirectory(*it);
    }

#ifdef TIMER
    std::cout << "Total load/parse time: " << totalLoadParse << "ms" << endl;
#endif
}
