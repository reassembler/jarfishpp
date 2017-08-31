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
      vector<string> extensions;

      bool searchInternal;
      bool searchStrings;

      std::vector<Hit> searchClass(std::istream&);

    public:
      unsigned int archivesScanned;
      unsigned int entriesTested;

      void addExtension(string ext) {
          extensions.push_back(ext);
      }

      void setSearchInternal(bool b) {
          this->searchInternal = true;
      }

      void setSearchStrings(bool b) {
          this->searchStrings = b;
      }

      Util() {
          searchInternal = false;
          searchStrings = false;
          extensions.push_back(".jar");

          archivesScanned = 0;
          entriesTested = 0;
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
