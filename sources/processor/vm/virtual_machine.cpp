#include "virtual_machine.h"
#include "processor/processor.h"
#include "processor/processor_control_block.h"
#include "processor/processor_state.h"

VirtualMachine::VirtualMachine(ProcessorStateWatcher processorStateWatcher, uint8_t systemInstructionPerStep)
    : _systemInstructionPerStep(systemInstructionPerStep)
    , _processorStateWatcher(std::move(processorStateWatcher))
{
}

bool VirtualMachine::RegisterProcedure(std::unique_ptr<ProcedureBase> procedure, uint8_t inputArgs, uint8_t outputArgs)
{
    ProcedureInfo info = ProcedureInfo { inputArgs, outputArgs, procedure.get() };
    const ProcedureId id = _procedureTable.RegisterProcedure(info);
    if (id == ProcedureId::Invalid) {
        return false;
    }
    _procedureOwner.emplace_back(std::move(procedure));
    return true;
}

void VirtualMachine::Run(Memory memory)
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