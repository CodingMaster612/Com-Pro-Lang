#pragma once

#include "Stringinstruction.hpp"
#include <cstdint>
#include <vector>
#include <optional>

namespace Stringinterpreter
{

    using namespace std;

    struct StringInterpreterRegisters
    {
    };
    
    
    void PrintStringInstruction(StringInterpreterRegisters &registers);
    typedef void (*StringInstructionFunction)(StringInterpreterRegisters &placeholder);

    extern StringInstructionFunction gInstructionFunctions[STRING_INSTRUCTIONS];

    class StringInterpreter
    {
    public:
        static void Run(Instruction *code, vector<string> arguments, string *result = nullptr);
    };

}