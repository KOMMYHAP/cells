#include "virtual_machine.h"
#include "processor/processor.h"
#include "processor/processor_control_block.h"

static_assert(ProcessorRegistryCount >= ProcedureInputArgsCountLimit + ProcedureOutputArgsCountLimit,
    "Register overflow! Processor use registries to pass args to or obtain args from procedure");

static_assert(std::is_trivial_v<ProcessorControlBlock>, "As part of memory view ProcessorControlBlock must be trivial");
static_assert(std::is_trivial_v<ProcessorInstruction>, "As part of memory view ProcessorInstruction must be trivial");
static_assert(std::is_trivial_v<ProcessorFlags>, "As part of memory view ProcessorFlags must be trivial");

VirtualMachine::VirtualMachine(ProcessorStateWatcher processorStateWatcher, uint8_t systemInstructionPerStep)
    : _systemInstructionPerStep(systemInstructionPerStep)
    , _processorStateWatcher(std::move(processorStateWatcher))
{
}

ProcedureId VirtualMachine::RegisterProcedure(ProcedureBase* procedure, uint8_t inputArgs, uint8_t outputArgs)
{
    ProcedureTableEntry info = ProcedureTableEntry { inputArgs, outputArgs, procedure };
    const ProcedureId id = _procedureTable.RegisterProcedure(info);
    return id;
}

void VirtualMachine::Run(ProcessorMemory memory)
{
    const auto [controlBlockRead, controlBlock] = memory.TryAccess<ProcessorControlBlock>();
    if (!controlBlockRead) {
        assert(false);
        return;
    }

    ProcessorContext context {
        _procedureTable,
        _processorStateWatcher,
        *controlBlock,
        memory,
    };
    Processor processor { _systemInstructionPerStep };
    processor.Execute(context);
}