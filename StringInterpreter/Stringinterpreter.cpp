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

    void StringInterpreter::Run(StringInstruction *code, vector<string> arguments, string *result)
    {
        StringInterpreterRegisters Stringregisters{.currentInstruction = code};

        if(result){
            Stringregisters.Stringstack.push_back("");
        }
    }
}