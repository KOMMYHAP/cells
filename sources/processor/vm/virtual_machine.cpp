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

std::optional<ProcessorContext> VirtualMachine::MakeProcessorContext(Memory memory)
{
    if (!memory.HasBytes<ProcessorControlBlock>()) {
        return {};
    }

    auto& controlBlock = memory.Get<ProcessorControlBlock>();

    ProcessorContext context {
        _procedureTable,
        controlBlock,
        memory
    };

    context.SetStateWatcher(&DebugBadProcessorState);

    return context;
}
