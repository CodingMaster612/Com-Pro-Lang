#pragma once

#include <cstdint>

namespace Stringinterpreter
{

    enum code : uint8_t
    {
        PRINT_STATE,
        LOAD_STATE,

        STRING_INSTRUCTIONS
    };

    class StringInstruction
    {
    public:
        code opcode;
        // uint8_t p1;
        // int16_t p2;
    };

}