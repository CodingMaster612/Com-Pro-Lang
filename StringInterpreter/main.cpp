#include <iostream>
#include <vector>
#include "StringInstruction.hpp"
#include "StringInterpreter.hpp"
#include "StringInterpreter.cpp"

using namespace std;
using namespace Stringinterpreter;

int main(int argc, char **argv)
{

    StringInstruction code[] = {

        StringInstruction{PRINT_STATE},

    };

    string StringValue = "";
    StringInterpreter::Run(code, &StringValue);

    cout << "\nResult: " << StringValue << "\ndone." << endl;

    return 0;
}