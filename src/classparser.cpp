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

std::vector<std::string> ClassParser::getPoolStrings() 
{
    std::vector<std::string> strings;

    int cpSize = pool.size();
    for (int i = 0; i < cpSize; i++) {
        Utf8Value* s = dynamic_cast<Utf8Value*>(pool[i]);

        if (s != NULL) {
            strings.push_back(s->value);
        }
    }

    return strings;
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

    // not all classes have a super class
    if (poolIndex > 0) {
        PoolValue *pv = getPoolValue(poolIndex);

        ClassValue* cv = dynamic_cast<ClassValue*>(pv);

        if (cv != NULL) {
            int i = cv->nameIndex;
        
            Utf8Value* name = dynamic_cast<Utf8Value*>(getPoolValue(i));

            if (name != NULL) {
                cname = name->value;
            }
        }
    }

    return cname;
}

std::string getTypeName(int type) {
    switch (type) {
    case CONSTANT_CLASS: 
        return "Constant Class";

    case CONSTANT_FIELD_REF:
        return "Constant Field Ref";

    case CONSTANT_METHOD_REF:
        return "Method Ref";

    case CONSTANT_INTERFACE_METHOD_REF:
        return "Interface Method Ref";

    case CONSTANT_NAME_AND_TYPE: 
        return "Name And Type";
        
    case CONSTANT_STRING_INFO: 
        return "String Info";
    
    case CONSTANT_INTEGER:
        return "Integer Info";

    case CONSTANT_FLOAT:
        return "Float Info";

    case CONSTANT_LONG:
        return "Long Info";

    case CONSTANT_DOUBLE:
        return "Double Info";

    case CONSTANT_UTF8:
        return "UTF8";
    }

    return "unknown";
}

int ClassParser::readConstantPool(std::istream& in) 
{
        int cpSize = readU2(in) - 1;

        for (int i = 0; i < cpSize; i++) {
            //std::cout << "cpIndex: " << (i + 1) << "/" << cpSize << " ";
            int type = in.get();

            //std::cout << getTypeName(type) << std::endl;

            switch (type) {

            case CONSTANT_CLASS: {
                // the value is the index of the class name in the constant pool
                ClassValue* cv = new ClassValue();
                pool.push_back(cv);

                cv->nameIndex = readU2(in);
            }
            break;

            case CONSTANT_FIELD_REF:
            case CONSTANT_METHOD_REF:
            case CONSTANT_INTERFACE_METHOD_REF:
            case CONSTANT_NAME_AND_TYPE: {
                RefValue* v = new RefValue(type);
                pool.push_back(v);
                v->classIndex = readU2(in);
                v->nameAndTypeIndex = readU2(in);
            }
            break;

            case CONSTANT_STRING_INFO: {
                StringInfoValue* v = new StringInfoValue();
                pool.push_back(v);

                v->stringIndex = readU2(in);
            }
            break;

            case CONSTANT_INTEGER:
            case CONSTANT_FLOAT:
                pool.push_back(new PoolValue(type));

                readU4(in);
                break;

            // the following two constant pool types take
            // up 2 slots in the constant pool. How fucking
            // shitty is that. Which means we need to add a phantom 
            // entry into the constant pool every time we find one of
            // these.
            case CONSTANT_LONG:
            case CONSTANT_DOUBLE:
                pool.push_back(new PoolValue(type));

                // add ridiculous phantom entry
                pool.push_back(new PoolValue(type));

                readU4(in);
                readU4(in);

                // increase the index because of phantom. Once again SHITTY
                i++;
                break;

            case CONSTANT_UTF8: {
                Utf8Value* v = new Utf8Value();
                pool.push_back(v);

                v->value = readUtf(in);
            }
            break;

            default:
                std::cerr << "Unknown type: " << type << std::endl;
                std::cerr << "reached unknown data in constant pool. We should quit now." << std::endl;
                break;
            }
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
