#pragma once

#include "Instruction.hpp"
#include <cstdint>
#include <vector>
#include <optional>

namespace bytecodeinterpreter
{

    using namespace std;

    struct InterpreterRegisters
    {
        vector<int16_t> stack;
        //vector<string> Test;
        //vector<uint8_t> Test;
        vector<Instruction *> returnAddressStack;
        Instruction *currentInstruction;
        size_t baseIndex;
    };

    typedef void (*InstructionFunction)(InterpreterRegisters &registers);

    void ExitInstruction(InterpreterRegisters &registers);
    void AddIntInstruction(InterpreterRegisters &registers);
    //AddString

    void PushIntInstruction(InterpreterRegisters &registers);
    //Push string

    void PopIntInstruction(InterpreterRegisters &registers);
    //pop string

    void PrintIntInstruction(InterpreterRegisters &registers);
    void PrintStringInstruction(InterpreterRegisters &registers);
    void CompareIntLessThanInstruction(InterpreterRegisters &registers);

    void LoadIntInstruction(InterpreterRegisters &registers);
    //load string
    void StoreIntInstruction(InterpreterRegisters &registers);
    //store string instruction
    void JumpByIfZeroInstruction(InterpreterRegisters &registers);
    void JumpByInstruction(InterpreterRegisters &registers);
    void LoadIntBasepointerRelativeInstruction(InterpreterRegisters &registers);
    //Load string Base pointer
    void StoreIntBasepointerRelativeInstruction(InterpreterRegisters &registers);
    // store String Base
    void CallInstruction(InterpreterRegisters &registers);
    void ReturnInstruction(InterpreterRegisters &registers);

    extern InstructionFunction gInstructionFunctions[NUM_INSTRUCTIONS];

    class BytecodeInterpreter
    {
    public:
        static void Run(Instruction *code, vector<int16_t> arguments, int16_t *result = nullptr);
    };

} /* namespace bytecodeinterpreter */