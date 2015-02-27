#include <iostream>
#include <sstream>
#include <vector>
#include <stdint.h>

#include "classparser.hpp"



unsigned int readU2(std::istream& in) 
{
    unsigned int t = in.get() << 8;

    t = t | in.get();

    return t;
}

unsigned long readU4(std::istream& in) 
{
    unsigned long t = in.get() << 24;

    t = t | (in.get() << 16);
    t = t | (in.get() << 8);
    t = t | in.get();

    return t;
}


std::string readUtf(std::istream& in) {
    int length = readU2(in);

    std::stringstream ss;

    for (int i = 0; i < length; i++) {
        // read byte group
        // first byte
        unsigned char ch = in.get();

        if ((ch >> 7) == 0) {
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


ClassParser::~ClassParser() {
    std::cout << "delete\n";

    int n = pool.size();

    for (int i = 0; i < n; i++) {
        if (pool[i] != NULL) {
            delete pool[i];
        }
    }
}


PoolValue* ClassParser::getPoolValue(int i) 
{
    return pool.at(i - 1);
}


std::string ClassParser::readClassName(std::istream& in)
{
    std::string cname("NAME NOT FOUND");

    int poolIndex = readU2(in);

    PoolValue *pv = getPoolValue(poolIndex);

    ClassValue* cv = dynamic_cast<ClassValue*>(pv);

    if (cv != NULL) {
        int i = cv->nameIndex;
    
        Utf8Value* name = dynamic_cast<Utf8Value*>(getPoolValue(i));

        if (name != NULL) {
            cname = name->value;
        }
    }

    return cname;
}


std::string ClassParser::readSuperClassName(std::istream& in)
{
    std::string cname("NAME NOT FOUND");

    int poolIndex = readU2(in);

    PoolValue *pv = getPoolValue(poolIndex);

    ClassValue* cv = dynamic_cast<ClassValue*>(pv);

    if (cv != NULL) {
        int i = cv->nameIndex;
    
        Utf8Value* name = dynamic_cast<Utf8Value*>(getPoolValue(i));

        if (name != NULL) {
            cname = name->value;
        }
    }

    return cname;
}


int ClassParser::readConstantPool(std::istream& in) 
{
        int cpSize = readU2(in) - 1;

        for (int i = 0; i < cpSize; i++) {
            int type = in.get();

            switch (type) {

            case CONSTANT_CLASS: {
                // the value is the index of the class name in the constant pool
                ClassValue* cv = new ClassValue();
                pool.push_back(cv);

                cv->nameIndex = readU2(in);
            }
            break;

            case CONSTANT_FIELD_REF:
            //std::cout << "Field Ref" << std::endl;
            case CONSTANT_METHOD_REF:
            //std::cout << "Method Ref" << std::endl;
            case CONSTANT_INTERFACE_METHOD_REF:
            //std::cout << "Interface Method Ref" << std::endl;
            case CONSTANT_NAME_AND_TYPE: {
            //std::cout << "Name And Type" << std::endl;
                RefValue* v = new RefValue(type);
                pool.push_back(v);
                v->classIndex = readU2(in);
                v->nameAndTypeIndex = readU2(in);
            }
            break;

            case CONSTANT_STRING_INFO: {
            //std::cout << "String Info" << std::endl;
                // index of std::string in constant pool
                StringInfoValue* v = new StringInfoValue();
                pool.push_back(v);

                v->stringIndex = readU2(in);
            }
            break;

            // skipping these values
            case CONSTANT_INTEGER:
            //std::cout << "Integer Info" << std::endl;
            case CONSTANT_FLOAT:
            //std::cout << "Float Info" << std::endl;
                pool.push_back(new PoolValue(type));

                readU4(in);
                break;

            // skipping these values
            case CONSTANT_LONG:
            //std::cout << "Long Info" << std::endl;
            case CONSTANT_DOUBLE:
            //std::cout << "Double Info" << std::endl;
                pool.push_back(new PoolValue(type));

                readU4(in);
                readU4(in);
                break;

            case CONSTANT_UTF8: {
            //std::cout << "UTF8" << std::endl;
                Utf8Value* v = new Utf8Value();
                pool.push_back(v);

                v->value = readUtf(in);

                //std::cout << "UTF8: " << v.value;
            }
            break;

            default:
                std::cout << "Unknown type: " << type << std::endl;
                // dont' know what it is, things will go badly after this
                std::cerr << "reached unknown data in constant pool. We should quit now." << std::endl;
                break;
            }

            //std::cout << std::endl;
        }    

    return pool.size();
}


void ClassParser::readVersion(std::istream& in)
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


    std::stringstream ss;

    ss << major << "." << minor;

    this->version = ss.str();
}


bool ClassParser::testSig(std::istream& in) 
{
    if (in.get() == 0xCA
            && in.get() == 0xFE
            && in.get() == 0xBA
            && in.get() == 0xBE) {
        return true;
    }

    return false;
}

void ClassParser::parseAll(std::istream& in) 
{
    readVersion(in);
    readConstantPool(in);
    readAccessFlags(in);
    this->className = readClassName(in);
    this->superClassName = readSuperClassName(in);
}
