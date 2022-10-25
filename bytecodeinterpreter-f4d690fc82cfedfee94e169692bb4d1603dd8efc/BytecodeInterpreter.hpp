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
        vector<string> StringLiteral;

        vector<Instruction *> returnAddressStack;
        Instruction *currentInstruction;
        size_t baseIndex;
    };

    typedef void (*InstructionFunction)(InterpreterRegisters &registers);
    typedef void (*InstructionFunction)(InterpreterRegisters &LiteralStringStatement);
    
    
    
    
    void ExitInstruction(InterpreterRegisters &registers);
    void AddIntInstruction(InterpreterRegisters &registers);
    void PushIntInstruction(InterpreterRegisters &registers);
    void PopIntInstruction(InterpreterRegisters &registers);
    void PrintIntInstruction(InterpreterRegisters &registers);
    void PrintStatementInstruction(InterpreterRegisters &LiteralStringStatement);
    void CompareIntLessThanInstruction(InterpreterRegisters &registers);
    void LoadIntInstruction(InterpreterRegisters &registers);
    void StoreIntInstruction(InterpreterRegisters &registers);
    void JumpByIfZeroInstruction(InterpreterRegisters &registers);
    void JumpByInstruction(InterpreterRegisters &registers);
    void LoadIntBasepointerRelativeInstruction(InterpreterRegisters &registers);
    void StoreIntBasepointerRelativeInstruction(InterpreterRegisters &registers);
    void CallInstruction(InterpreterRegisters &registers);
    void ReturnInstruction(InterpreterRegisters &registers);
    //void PrintStateInstruction(InterpreterRegisters &registers);
    void AddStringInstruction(InterpreterRegisters &registers);
    void PushStringInstruction(InterpreterRegisters &registers);
    void PopStringInstruction(InterpreterRegisters &registers);
    void LoadStringInstruction(InterpreterRegisters &registers);
    void StoreStringInstruction(InterpreterRegisters &registers);
    void LoadStringBasepointerRelativeInstruction(InterpreterRegisters &registers);
    void StoreStringBasepointerRelativeInstruction(InterpreterRegisters &registers);
    extern InstructionFunction gInstructionFunctionsInteger[NUM_INSTRUCTIONS];
    
   
    class BytecodeInterpreter
    {
    public:
        static void Run(Instruction *code, vector<int16_t> arguments, int16_t *result = nullptr);
    };

} /* namespace bytecodeinterpreter */