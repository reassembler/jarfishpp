#include <iostream>

using namespace std;

class ClassParser {
    public:
        bool testSig(istream& in);
        void readVersion(istream& in, string& version);
};
