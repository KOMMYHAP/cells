#include "processor.h"
#include "flags.h"
#include "processor_profile_category.h"

ProcessorContext::ProcessorContext(const ProcedureTable& procedureTable, ProcessorControlBlock& controlBlock, const Memory& memory)
    : _procedureTable(procedureTable)
    , _controlBlock(controlBlock)
    , _memory(memory)
{
}

void ProcessorContext::SetInvalidCommandTrap(std::function<void()> trap)
{
    _invalidCommandTrap = std::move(trap);
}

bool ProcessorContext::HasFlag(ProcessorFlags flag) const
{
    return common::HasFlag(_controlBlock.flags, flag);
}

void ProcessorContext::SetFlag(ProcessorFlags flag)
{
    common::SetFlag(_controlBlock.flags, flag);

    if (flag == ProcessorFlags::InvalidCommand && _invalidCommandTrap) {
        _invalidCommandTrap();
    }
}

void ProcessorContext::ResetFlag(ProcessorFlags flag)
{
    common::ResetFlag(_controlBlock.flags, flag);
}

std::optional<ProcedureContext> ProcessorContext::MakeProcedureContext(ProcedureId id)
{
    const ProcedureInfo* info = _procedureTable.FindProcedure(id);
    if (!info) {
        SetFlag(ProcessorFlags::InvalidCommand);
        return {};
    }

    const uint8_t argsCount = info->inputArgsCount + info->outputArgsCount;
    const uint8_t bytesRequired = argsCount * ProcessorUnitSize;
    if (!_memory.HasBytes(bytesRequired)) {
        SetFlag(ProcessorFlags::InvalidCommand);
        return {};
    }

    const Memory memory = _memory.MakeSubSpan(bytesRequired);
    return ProcedureContext(*this, memory);
}

void Processor::Execute(ProcessorContext& context)
{
    common::ProfileScope processorProfileScope { "Processor", ProcessorProfileCategory };

    TryProcessInstruction(context);
}

std::optional<ProcessorInstruction> Processor::TryProcessInstruction(ProcessorContext& context)
{
    if (context.HasFlag(ProcessorFlags::InvalidCommand)) {
        return {};
    }

    // todo
    return {};
}
