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

      bool searchInternal;
      bool searchStrings;

      std::vector<Hit> searchClass(std::istream&);

      int hits;

    public:
      unsigned int archivesScanned;
      unsigned int entriesTested;

      vector<string> extensions;

      void setSearchInternal(bool b) {
          this->searchInternal = true;
      }

      void setSearchStrings(bool b) {
          this->searchStrings = b;
      }

      Util() {
          hits = 0;

          searchInternal = false;
          searchStrings = false;
          extensions.push_back(".jar");

          archivesScanned = 0;
          entriesTested = 0;
      }

      int hitCount() {
          return hits;
      }    

      void listDirectory(std::string path);
      void scan(std::vector<std::string> paths);

      void addQuery(string query) {
          queries.push_back(query);
      }

      bool hasQuery() {
          return queries.size() > 0;
      }    
};

#endif
