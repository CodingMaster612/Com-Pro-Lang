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
        
        vector<string> stack;
        vector<StringInstruction *> returnStringAddress;
        StringInstruction *currentInstruction;
        size_t baseIndex;
    };
    
    
    void PrintStringInstruction(StringInterpreterRegisters &placeholder);
    typedef void (*StringInstructionFunction)(StringInterpreterRegisters &placeholder);

    extern StringInstructionFunction gInstructionFunctions[STRING_INSTRUCTIONS];

    class StringInterpreter
    {
    public:
        static void Run(StringInstruction *code, vector<string> arguments, string *result = nullptr);
    };

}