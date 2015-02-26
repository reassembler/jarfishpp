#include <iostream>

using namespace std;

class ClassParser {
    private:

    public:
        bool testSig(istream& in);
        void readVersion(istream& in, string& version);
        int readConstantPoolSize(istream& in);
};
