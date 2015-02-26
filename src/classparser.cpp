#include <iostream>
#include <sstream>
#include <vector>
#include <stdint.h>

#include "classparser.hpp"

using namespace std;


unsigned int readU2(istream& in) 
{
    unsigned int t = in.get() << 8;

    t = t | in.get();

    return t;
}

unsigned long readU4(istream& in) 
{
    unsigned long t = in.get() << 24;

    t = t | (in.get() << 16);
    t = t | (in.get() << 8);
    t = t | in.get();

    return t;
}



string readUtf(istream& in) {
    int length = readU2(in);

    stringstream ss;

    for (int i = 0; i < length; i++) {
        // read byte group
        // first byte
        unsigned char ch = in.get();

        if ((ch & 128) == 0) {
            // 0xxxxxxx one byte group
            ss << ch;
        }
        else if ((ch >> 5) == 6) {
            // 110xxxxx two-byte group
            ss << "X";
            in.get();
            i++;
        }
        else if ((ch >> 4) == 14) {
            // 1110xxxx three-byte group
            ss << "X";
            in.get();
            in.get();

            i++;
            i++;
        }
    }

    return ss.str();
}

string ClassParser::readClassName(istream& in)
{
    int poolIndex = readU2(in);

    cout << "trace " << poolIndex << endl;
    cout << "trace " << pool.size() << endl;
    
    PoolValue pv = pool.at(poolIndex);

    PoolValue *ppv = &pv;

    if (ppv == NULL ) {
        cout << "ITS NULL" << endl;
    }

    //cout << "PPV: " << ppv << endl;
    ClassValue* pcv = static_cast<ClassValue*>(ppv);

    if (pcv != NULL) {

    int i = pcv->nameIndex;
    cout << "trace A" << endl;

    cout << "index: " << i << endl;
    }

    return string("HELLO");

}

int ClassParser::readConstantPool(istream& in) 
{
        int cpSize = readU2(in) - 1;

        for (int i = 0; i < cpSize; i++) {
            int type = in.get();

            //cout << "index: " << i << " ";

            switch (type) {

            case CONSTANT_CLASS: {
                // the value is the index of the class name in the constant pool
                ClassValue cv = ClassValue();
                pool.push_back(cv);

                cv.nameIndex = readU2(in);

                cout << "INDEXCC: " << cv.nameIndex << endl;
            }
            //cout << "Class" << endl;
            break;

            case CONSTANT_FIELD_REF:
            //cout << "Field Ref" << endl;
            case CONSTANT_METHOD_REF:
            //cout << "Method Ref" << endl;
            case CONSTANT_INTERFACE_METHOD_REF:
            //cout << "Interface Method Ref" << endl;
            case CONSTANT_NAME_AND_TYPE: {
            //cout << "Name And Type" << endl;
                RefValue v = RefValue(type);
                pool.push_back(v);
                v.classIndex = readU2(in);
                v.nameAndTypeIndex = readU2(in);
            }
            break;

            case CONSTANT_STRING_INFO: {
            //cout << "String Info" << endl;
                // index of string in constant pool
                StringInfoValue v = StringInfoValue();
                pool.push_back(v);

                v.stringIndex = readU2(in);
            }
            break;

            // skipping these values
            case CONSTANT_INTEGER:
            //cout << "Integer Info" << endl;
            case CONSTANT_FLOAT:
            //cout << "Float Info" << endl;
                pool.push_back(PoolValue(type));

                readU4(in);
                break;

            // skipping these values
            case CONSTANT_LONG:
            //cout << "Long Info" << endl;
            case CONSTANT_DOUBLE:
            //cout << "Double Info" << endl;
                pool.push_back(PoolValue(type));

                readU4(in);
                readU4(in);
                break;

            case CONSTANT_UTF8: {
            //cout << "UTF8" << endl;
                Utf8Value v = Utf8Value();
                pool.push_back(v);

                v.value = readUtf(in);

                //cout << "UTF8: " << v.value;
            }
            break;

            default:
                cout << "Unknown type: " << type << endl;
                // dont' know what it is, things will go badly after this
                cerr << "reached unknown data in constant pool. We should quit now." << endl;
                break;
            }

            //cout << endl;
        }    

    return pool.size();
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
