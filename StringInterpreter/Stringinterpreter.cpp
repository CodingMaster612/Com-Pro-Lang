#include "Stringinterpreter.hpp"
//#include "C:\\Users\\Tim\\Documents\\GitHub\\Com-Pro-Lang\\simpleparser-e5c86326aaec52dcaf3a95c198c95f2c0195a161\Tokenizer.cpp"
//#include "C:\Users\Tim\Documents\GitHub\Com-Pro-Lang\simpleparser-e5c86326aaec52dcaf3a95c198c95f2c0195a161\Tokenizer.hpp"

#include <iostream>
#include <vector>

namespace Stringinterpreter
{

    using namespace std;

    StringInstructionFunction gInstructionFunctions[STRING_INSTRUCTIONS] = {
        // add
        PrintStringInstruction,
    };

   void StringInterpreter::Run(Instruction *code, vector<string> arguments, string *result)
    {
        StringInterpreterRegisters registers{.currentInstruction = code};
    }