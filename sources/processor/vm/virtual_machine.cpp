#include "virtual_machine.h"
#include "processor/processor.h"
#include "processor/processor_control_block.h"

#include <windows.h>
#include <debugapi.h>

static void DebugInvalidCommandTrap()
{
    DebugBreak();
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

    context.SetInvalidCommandTrap(&DebugInvalidCommandTrap);

    return context;
}
