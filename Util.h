#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <vector>

const char kPathSeparator =
#ifdef _WIN32
                            '\\';
#else
                            '/';
#endif



using namespace std;

class Util {
    public:
      static bool isDirectory(string path);
      static void listDirectory(std::string path);
      static void scan(std::vector<std::string> paths);
};

#endif
