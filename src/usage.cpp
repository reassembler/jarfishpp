#include <iostream>

using namespace std;


void show_usage(const char * errorMessage) {
    if (errorMessage != NULL) {
        cerr << "Error: " << errorMessage << endl;
    }
    
    cout << "use: jarfish <option>* (<action> [arg])+ <path>+" << endl;
    cout << endl;
    cout << "Action      Description      Arg                          Multiples Actions allowed?" << endl;
    cout << "--findc     Find a class     Class Name                   Yes " << endl;
    cout << "                             org.assemble.Person" << endl;
    cout << "                             Person" << endl;
    cout << "                             org/assembler/Person" << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << endl;

    cout << "Options" << endl;

    cout << "--deep - No arguments" << endl;
    cout << "  Look inside of application archives e.g. wars when searching." << endl;
    cout << endl;

    cout << "--ext - Has arguments, option can appear more than once." << endl;
    cout << "  Add a file type to search for class files in." << endl;
    cout << "  e.g. --ext zip" << endl;
    cout << "  e.g. --ext zip --ext bin" << endl;
    cout << "  Note: File type must be a valid zip file no matter what the extension is." << endl;
    cout << endl;

    cout << "--internal - No arguments" << endl;
    cout << "  Look inside the class file for the class name instead of depending on the file name and path." << endl;
    cout << "  e.g. --internal" << endl;
    cout << "  Note: This will make searches take much longer." << endl;
    cout << endl;

}
