#include <iostream>
#include <fstream>
#include <string>

#include "classparser.hpp"

using namespace std;

int main(int argc, char **argv)
{

    if (argc == 1) {
        cerr << "no input files" << endl;
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
            parser.parseAll(in);
            std::cout << parser.version << std::endl;
            std::cout << parser.className << std::endl;
            std::cout << parser.superClassName << std::endl;
        }
        else {
            std::cout << "Failed to find signature in class file" << std::endl;
        }

        in.close();
    }

    return 0;
}
