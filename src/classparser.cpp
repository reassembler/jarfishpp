#include <iostream>
#include <sstream>
#include <stdint.h>

#include "classparser.hpp"

using namespace std;

void ClassParser::readVersion(istream& in, string& version)
{
    unsigned int major = 0;
    unsigned int minor = 0;
    unsigned int t;

    minor = in.get();

    minor << 8;

    t = in.get();

    minor = minor | t;


    major = in.get();

    major << 8;

    t = in.get();

    major = major | t;


    stringstream ss;

    ss << major << "." << minor;

    version.append(ss.str());
    
}

bool ClassParser::testSig(istream& in) 
{
    if (in.get() == 0xCA
            && in.get() == 0xFE
            && in.get() == 0xBA
            && in.get() == 0xBE) {
        return true;
    }

    return false;
}
