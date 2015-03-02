#include <iostream>

using namespace std;


void show_usage(const char * errorMessage) {
    cout << "jarfish " << VERSION << endl;

    if (errorMessage != NULL) {
        cerr << "Error: " << errorMessage << endl;
    }
    
    cout << "use: jarfish <option>* (<action> [arg])+ <path>+" << endl;
    cout << endl;
    cout << "Action      Description      Arg                          Multiples Actions allowed?" << endl;
    cout << "--findc     Find a class     Class Name                   Yes " << endl;
    cout << "                             e.g. org.assemble.Person" << endl;
    cout << "                             e.g. Person" << endl;
    cout << "                             e.g. org/assembler/Person" << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "--finds     Find a String    String to find               Yes " << endl;
    cout << "             inside the      e.g. Daisy" << endl;
    cout << "             class file      e.g. Hello, World!" << endl;
    cout << endl;
    cout << "EX: jarfish --finds 'Mo Money' ." << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << endl;

    cout << "Options" << endl;

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

    cout << "--showStats - No arguments" << endl;
    cout << "  Show scan stats at end of search." << endl;
    cout << endl;

}
