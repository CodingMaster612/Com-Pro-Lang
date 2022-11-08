#include "Stringinterpreter.hpp"

#include <iostream>
#include <vector>

namespace Stringinterpreter
{

    using namespace std;

    StringInstructionFunction gInstructionFunctions[STRING_INSTRUCTIONS] = {
        // add
        PrintStringInstruction,
    };

    void StringInterpreter::Run(StringInstruction *code,  string *result)
    {
        StringInterpreterRegisters StringRegisters{.currentInstruction = code};

        if (result)
        {
            StringRegisters.String.push_back("");
        }
        StringRegisters.String.insert(StringRegisters.String.end(), StringRegisters.String.begin(), StringRegisters.String.end());
        
        StringRegisters.String.push_back(""); // push old baseIndex.
        StringRegisters.returnStringAddress.push_back(nullptr);
        StringRegisters.baseIndex = StringRegisters.String.size();

        while (StringRegisters.currentInstruction != nullptr)
        {
            gInstructionFunctions[StringRegisters.currentInstruction->opcode](StringRegisters);
        }
    
        size_t StringArgs = StringRegisters.String.size();
        while (StringArgs--)
        {
            StringRegisters.String.pop_back();
        }

        if (result)
        {
            *result = StringRegisters.String[0];
        }
    
    }   
    //! PRINT_STATE
    void PrintStringInstruction(StringInterpreterRegisters &StringRegisters)
    {
        string strings = StringRegisters.String.back();
        StringRegisters.String.pop_back();
        cout << "String Printed: " << strings << endl;
        ++StringRegisters.currentInstruction;
    }
    
}