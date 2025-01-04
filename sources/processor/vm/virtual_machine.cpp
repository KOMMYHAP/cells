#include "virtual_machine.h"

#include "processor/processor.h"
#include "processor/processor_control_block.h"

static_assert(ProcessorStackSize >= std::max(ProcedureInputArgsCountLimit, ProcedureOutputArgsCountLimit),
    "Register overflow! Processor use registries to pass args to or obtain args from procedure");

static_assert(std::is_trivial_v<ProcessorControlBlock> && std::has_unique_object_representations_v<ProcessorControlBlock> && alignof(ProcessorControlBlock) == 1,
    "As part of memory view ProcessorControlBlock must be tightly packed and should not have a padding");

static_assert(sizeof(ProcessorControlBlock::pendingProcedureId) == sizeof(std::underlying_type_t<ProcedureId>));
static_assert(sizeof(ProcessorControlBlock::state) == sizeof(std::underlying_type_t<ProcessorState>));

static_assert(std::is_enum_v<ProcessorInstruction>, "As part of memory view ProcessorInstruction must be trivial");
static_assert(std::is_enum_v<ProcessorFlags>, "As part of memory view ProcessorFlags must be trivial");

ProcedureId VirtualMachine::RegisterProcedure(ProcedureBase* procedure, uint8_t inputArgs, uint8_t outputArgs)
{
    const auto info = ProcedureTableEntry { inputArgs, outputArgs, procedure };
    const ProcedureId id = _procedureTable.RegisterProcedure(info);
    return id;
}

void VirtualMachine::Run(ProcessorMemory memory, ProcessorUserData userData /*= {}*/)
{
    const auto [controlBlockRead, controlBlock] = memory.TryAccess<ProcessorControlBlock>();
    ASSERT(controlBlockRead, "Invalid control block");

    ProcessorContext::Params params {
        &_procedureTable,
        controlBlock,
        userData,
        memory
    };
    ProcessorContext context { std::move(params) };

    ProcessorDebugger* debugger { nullptr };
    if (_debugger && _debugger->ShouldAttachDebugger(context)) {
        debugger = _debugger;
    }

    Processor processor { context, debugger };
    processor.Execute();
}

void VirtualMachine::CompleteDeferredExecution(ProcessorMemory memory, const ProcedureContext& procedureContext)
{
    const auto [controlBlockRead, controlBlock] = memory.TryAccess<ProcessorControlBlock>();
    ASSERT(controlBlockRead, "Invalid control block");

    ProcessorContext::Params params {
        &_procedureTable,
        controlBlock,
        procedureContext.GetUserData(),
        memory
    };
    ProcessorContext processorContext { std::move(params) };
    ProcessorDebugger* debugger { nullptr };
    if (_debugger && _debugger->ShouldAttachDebugger(processorContext)) {
        debugger = _debugger;
    }

    Processor processor { processorContext, debugger };
    processor.CompletePendingProcedure(procedureContext);
}

void VirtualMachine::SetDebugger(ProcessorDebugger* debugger)
{
    _debugger = debugger;
}
