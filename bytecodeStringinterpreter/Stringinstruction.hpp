#pragma once

#include <cstdint>

namespace bytecodestringinterpreter
{

    enum Opcode : uint8_t
    {
        Print_String,
    };

    class StringInstruction
    {

    public:
        Opcode opcode;
        uint8_t x;
        
    };

}