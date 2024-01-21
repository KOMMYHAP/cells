#include "virtual_machine.h"
#include "processor/processor.h"
#include "processor/processor_control_block.h"

static void DebugBadProcessorState(ProcessorState state)
{
    if (state == ProcessorState::Good) {
        return;
    }
    int breakOnMe { 42 };
}

VirtualMachine::VirtualMachine(uint8_t systemInstructionPerStep)
    : _systemInstructionPerStep(systemInstructionPerStep)
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
        *controlBlock,
        memory
    };
    context.SetStateWatcher(&DebugBadProcessorState);

    Processor processor { _systemInstructionPerStep };
    processor.Execute(context);
}