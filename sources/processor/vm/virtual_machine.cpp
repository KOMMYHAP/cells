#include "virtual_machine.h"

#include "processor/processor.h"
#include "processor/processor_control_block.h"

static_assert(ProcessorStackSize >= std::max(ProcedureInputArgsCountLimit, ProcedureOutputArgsCountLimit),
    "Register overflow! Processor use registries to pass args to or obtain args from procedure");

static_assert(sizeof(ProcessorControlBlock::pendingProcedureId) == sizeof(std::underlying_type_t<ProcedureId>));
static_assert(sizeof(ProcessorControlBlock::state) == sizeof(std::underlying_type_t<ProcessorState>));

static_assert(std::is_trivial_v<ProcessorControlBlock>, "As part of memory view ProcessorControlBlock must be trivial");
static_assert(std::is_trivial_v<ProcessorInstruction>, "As part of memory view ProcessorInstruction must be trivial");
static_assert(std::is_trivial_v<ProcessorFlags>, "As part of memory view ProcessorFlags must be trivial");

ProcedureId VirtualMachine::RegisterProcedure(ProcedureBase* procedure, uint8_t inputArgs, uint8_t outputArgs)
{
    const auto info = ProcedureTableEntry { inputArgs, outputArgs, procedure };
    const ProcedureId id = _procedureTable.RegisterProcedure(info);
    return id;
}

void VirtualMachine::Run(ProcessorMemory memory, ProcessorUserData userData /*= {}*/)
{
    const auto [controlBlockRead, controlBlock] = memory.TryAccess<ProcessorControlBlock>();
    ASSERT(controlBlockRead);

    ProcessorContext::Params params {
        &_procedureTable,
        controlBlock,
        userData,
        memory
    };
    ProcessorContext context { std::move(params) };
    Processor processor;

    const bool debuggerEnabled = _debugger && _debugger->ShouldAttachDebugger(context);
    if (debuggerEnabled) {
        _debugger->AttachDebugger(context);
        processor.SetDebugger(_debugger);
    }

    processor.Execute(context);

    if (debuggerEnabled) {
        processor.SetDebugger(nullptr);
        _debugger->DetachDebugger(context);
    }
}

void VirtualMachine::CompleteDeferredExecution(ProcessorMemory memory, const ProcedureContext& procedureContext)
{
    const auto [controlBlockRead, controlBlock] = memory.TryAccess<ProcessorControlBlock>();
    ASSERT(controlBlockRead);

    ProcessorContext::Params params {
        &_procedureTable,
        controlBlock,
        procedureContext.GetUserData(),
        memory
    };
    ProcessorContext processorContext { std::move(params) };
    Processor processor;

    const bool debuggerEnabled = _debugger && _debugger->ShouldAttachDebugger(processorContext);
    if (debuggerEnabled) {
        _debugger->AttachDebugger(processorContext);
        processor.SetDebugger(_debugger);
    }

    processor.CompletePendingProcedure(processorContext, procedureContext);

    if (debuggerEnabled) {
        processor.SetDebugger(nullptr);
        _debugger->DetachDebugger(processorContext);
    }
}

void VirtualMachine::SetDebugger(ProcessorDebugger* debugger)
{
    _debugger = debugger;
}
