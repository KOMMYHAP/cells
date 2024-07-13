#pragma once
#include "processor_context.h"
#include "processor_instruction.h"

class ProcessorDebugger;

class Processor {
public:
    void SetDebugger(ProcessorDebugger* debugger);
    
    void Execute(ProcessorContext& context);
    void CompletePendingProcedure(ProcessorContext& context, const ProcedureContext& procedureContext);

private:
    bool StartProcedure(ProcessorContext& context, ProcedureId id);
    
    std::optional<ProcessorInstruction> ProcessInstruction(ProcessorContext& context);

    struct TwoOperandsContext {
        ProcessorInstruction instruction;
        std::uint8_t operand1;
        std::uint8_t operand2;
    };
    bool ProcessTwoOperands(TwoOperandsContext instructionContext, ProcessorContext& context);

    struct OneOperandContext {
        ProcessorInstruction instruction;
        std::uint8_t operand1;
    };
    bool ProcessOneOperand(OneOperandContext instructionContext, ProcessorContext& context);

    struct FlagsContext {
        ProcessorInstruction instruction;
        std::uint8_t operand1;
        std::uint8_t operand2;
    };
    void UpdateFlags(FlagsContext flagsContext, ProcessorContext& context);

    ProcessorDebugger* _debugger { nullptr };
};