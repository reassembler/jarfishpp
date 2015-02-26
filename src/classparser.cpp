#include <iostream>
#include <sstream>
#include <vector>
#include <stdint.h>

#include "classparser.hpp"

using namespace std;

int readU2(istream& in) 
{
    unsigned int t;

    t = in.get();

    t = t << 8;

    t = t | in.get();

    return t;
}

/*

Object readValue(int type, DataInputStream din) throws IOException {
        switch (type) {
        case 7:
            // CONSTANT_Class 7
            // the value is the index of the class name in the constant pool
            return new Integer(readU2(din));

        case 9:
            // CONSTANT_Fieldref 9
        case 10:
            // CONSTANT_Methodref 10
        case 11:
            // CONSTANT_InterfaceMethodref 11
        case 12:
            // CONSTANT_NameAndType 12
            
            readU2(din);
            readU2(din);
            break;

        case 8:
            // CONSTANT_String 8
            return new Integer(readU2(din));

        case 3:
            // CONSTANT_Integer 3
        case 4:
            // CONSTANT_Float 4
            readU4(din);
            break;

        case 5:
            // CONSTANT_Long 5
        case 6:
            // CONSTANT_Double 6
            readU4(din);
            readU4(din);
            break;

        case 1:
            // CONSTANT_Utf8 1
            String val = din.readUTF();
            return val;

        default:
            return null;

        }
        
        return null;
    }


*/


int ClassParser::readConstantPoolSize(istream& in) 
{
        int cpSize = readU2(in) - 1;

        vector<string> entries;
        for (int i = 0; i < cpSize; i++) {
            int type = in.get();
        }    



/*
            Object value = readValue(type, din);
            entries.add(new ConstantEntry(type, value));
            
            String typeName = ConstantEntry.NAMES[type];
            if (typeName.equals("CONSTANT_Long") || typeName.equals("CONSTANT_Double")) {
                // skip phantom entries for long and double
                entries.add(new ConstantEntry(0, null));
                i++; 
            }
        }
        
        cm.setEntries(entries);

*/
    return cpSize;



}

void ClassParser::readVersion(istream& in, string& version)
{
    unsigned int major = 0;
    unsigned int minor = 0;
    unsigned int t;

    minor = in.get();

    minor = minor << 8;

    t = in.get();

    minor = minor | t;


    major = in.get();

    major = major << 8;

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
