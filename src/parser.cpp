#include <iostream>
#include <fstream>
#include <string>

#include "classparser.hpp"

using namespace std;

int main(int argc, char **argv)
{

    if (argc == 1) {
        cerr << "no files supplied" << endl;
        return 1;
        //EARLY OUT
    }
    
    for (int i = 1; i < argc; i++) {
        string name = argv[i];

        cout << "filename: " << name << endl;

        ifstream in;

        in.open(name.c_str(), ios_base::binary);

        ClassParser parser = ClassParser();

        if (parser.testSig(in)) {
            cout << "sig matched" << endl;

            string version;

            parser.readVersion(in, version);

            cout << "version: " << version << endl;

            cout << "constant pool size: " << parser.readConstantPoolSize(in) << endl;
        }
        else {
            cout << "no sig" << endl;
        }

        in.close();
    }

    return 0;
}
