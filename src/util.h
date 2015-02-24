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
    private: 
      bool isArchive(string path);
      bool isDirectory(string path);

    public:
      vector<string> extensions;

      void listDirectory(std::string path);
      void scan(std::vector<std::string> paths);

      Util() {
          extensions.push_back(".jar");
      }
};

#endif
