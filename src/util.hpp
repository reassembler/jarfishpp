#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <vector>

#include "hit.hpp"

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
      vector<Hit> matchesQuery(string test);
      void searchArchive(string fileName);

      vector<string> queries;

      vector<Hit> hits;


    public:
      vector<string> extensions;

      int hitCount() {
          return hits.size();
      }    

      void listDirectory(std::string path);
      void scan(std::vector<std::string> paths);

      void addQuery(string query) {
          queries.push_back(query);
      }

      bool hasQuery() {
          return queries.size() > 0;
      }    

      Util() {
          extensions.push_back(".jar");
      }
};

#endif
