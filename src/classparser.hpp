#include <iostream>
#include <vector>
#include <string>

using namespace std;



const int CONSTANT_UTF8 = 1;
const int CONSTANT_INTEGER = 3;
const int CONSTANT_FLOAT = 4;
const int CONSTANT_LONG = 5;
const int CONSTANT_DOUBLE = 6;
const int CONSTANT_CLASS = 7;
const int CONSTANT_STRING_INFO = 8;
const int CONSTANT_FIELD_REF = 9;
const int CONSTANT_METHOD_REF = 10;
const int CONSTANT_INTERFACE_METHOD_REF = 11;
const int CONSTANT_NAME_AND_TYPE = 12;
const int CONSTANT_METHOD_HANDLE = 15;
const int CONSTANT_METHOD_TYPE = 16;
const int CONSTANT_INVOKE_DYNAMIC = 18;



class PoolValue {
  public:
      int poolIndex;
      int type;

      PoolValue(int type) {
          this->type = type;
      }

      virtual ~PoolValue() {}
};

class Utf8Value : public PoolValue {
    public:
        string value;

        Utf8Value() : PoolValue(CONSTANT_UTF8) {};
};

// CONSTANT_Fieldref_info
// CONSTANT_Methodref_info
// CONSTANT_InterfaceMethodref_info
class RefValue : public PoolValue {
    public:
        int classIndex;
        int nameAndTypeIndex;
        RefValue(int val) : PoolValue(val) {};
};

// CONSTANT_Class_info
class ClassValue : public PoolValue {
    public:
        int nameIndex;
        ClassValue() : PoolValue(CONSTANT_CLASS) {};
};

// CONSTANT_String_info 
class StringInfoValue : public PoolValue {
    public:
        int stringIndex;

        StringInfoValue() : PoolValue(CONSTANT_STRING_INFO) {}
};



// CONSTANT_Integer_info
class IntegerValue : public PoolValue {
    public:
        int value;
        IntegerValue() : PoolValue(CONSTANT_INTEGER) {}
};




class ClassParser {
    private:
        vector<PoolValue> pool;

    public:
        bool testSig(istream& in);
        void readVersion(istream& in, string& version);
        int readConstantPool(istream& in);
        int readAccessFlags(istream& in) { 
            in.get();
            in.get();
        }
        string readClassName(istream& in);
};
