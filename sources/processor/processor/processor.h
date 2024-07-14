#pragma once
#include "processor_context.h"
#include "processor_instruction.h"

class ProcessorDebugger;

class Processor {
public:
    Processor(ProcessorContext& context, ProcessorDebugger* debugger);
    Processor(const Processor&) = delete;
    Processor& operator=(const Processor&) = delete;
    Processor(Processor&&) noexcept = delete;
    Processor& operator=(Processor&&) noexcept = delete;
    ~Processor() noexcept;

    void Execute();
    void CompletePendingProcedure(const ProcedureContext& procedureContext);

private:
    bool RunProcedure(ProcedureId id);
    bool CompleteProcedure(const ProcedureContext& procedureContext);
    void DeferProcedure(const ProcedureContext& procedureContext);

    std::optional<ProcessorInstruction> ProcessInstruction();

    struct TwoOperandsContext {
        ProcessorInstruction instruction;
        std::uint8_t operand1;
        std::uint8_t operand2;
    };
    bool ProcessTwoOperands(TwoOperandsContext instructionContext);

    struct OneOperandContext {
        ProcessorInstruction instruction;
        std::uint8_t operand1;
    };
    bool ProcessOneOperand(OneOperandContext instructionContext);

    struct FlagsContext {
        ProcessorInstruction instruction;
        std::uint8_t operand1;
        std::uint8_t operand2;
    };
    void UpdateFlags(FlagsContext flagsContext);

    ProcessorDebugger* _debugger { nullptr };
    gsl::not_null<ProcessorContext*> _processorContext;
};