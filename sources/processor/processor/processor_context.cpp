#include "processor_context.h"

#include "flags.h"
#include "processor_state.h"

ProcessorContext::ProcessorContext(const ProcedureTable& procedureTable, const ProcessorStateWatcher& stateWatcher, ProcessorControlBlock& controlBlock, const ProcessorMemory& memory)
    : _procedureTable(procedureTable)
    , _controlBlock(controlBlock)
    , _memory(memory.MakeSubSpan(_controlBlock.nextCommand))
    , _watcher(stateWatcher)
    , _stack(_controlBlock.stack, _controlBlock.stackOffset)
{
}

bool ProcessorContext::HasFlag(ProcessorFlags flag) const
{
    return common::HasFlag(_controlBlock.flags, flag);
}

void ProcessorContext::SetFlag(ProcessorFlags flag)
{
    common::SetFlag(_controlBlock.flags, flag);
}

void ProcessorContext::ResetFlag(ProcessorFlags flag)
{
    common::ResetFlag(_controlBlock.flags, flag);
}

bool ProcessorContext::IsState(ProcessorState state) const
{
    return _controlBlock.state == static_cast<uint8_t>(state);
}

void ProcessorContext::SetState(ProcessorState state)
{
    _controlBlock.state = static_cast<uint8_t>(state);
    if (_watcher) {
        _watcher(state);
    }
}

bool ProcessorContext::SetCommandPointer(uint8_t nextCommand)
{
    if (!_memory.HasBytes(nextCommand)) {
        return false;
    }
    _controlBlock.nextCommand = nextCommand;
    return true;
}

bool ProcessorContext::MoveCommandPointer(uint8_t offset)
{
    if (_controlBlock.nextCommand + offset <= _controlBlock.nextCommand) {
        SetState(ProcessorState::InvalidCommand);
        return false;
    }
    return SetCommandPointer(_controlBlock.nextCommand + offset);
}

bool ProcessorContext::WriteRegistry(uint8_t index, std::byte data)
{
    if (index >= _controlBlock.registry.size()) {
        SetState(ProcessorState::InvalidCommand);
        return false;
    }
    _controlBlock.registry[index] = data;
    return true;
}

std::pair<bool, std::byte> ProcessorContext::ReadRegistry(uint8_t index)
{
    if (index >= _controlBlock.registry.size()) {
        SetState(ProcessorState::InvalidCommand);
        return { false, std::byte {} };
    }
    return { true, _controlBlock.registry[index] };
}

bool ProcessorContext::RunProcedure(ProcedureId id)
{
    const ProcedureTableEntry* info = _procedureTable.FindProcedure(id);
    if (!info) {
        SetState(ProcessorState::UnknownProcedure);
        return false;
    }

    const uint8_t inputArgsCount = info->inputArgsCount;
    const uint8_t outputArgsCount = info->outputArgsCount;
    const uint8_t initialStackOffset = _controlBlock.stackOffset;

    if (initialStackOffset < inputArgsCount) {
        SetState(ProcessorState::InvalidCommand);
        return false;
    }

    ProcedureContext procedureContext { *this, _stack, inputArgsCount, outputArgsCount };
    info->procedure->Execute(procedureContext);

    uint8_t expectedStackOffset = 0;
    if (outputArgsCount > inputArgsCount) {
        expectedStackOffset = initialStackOffset + (outputArgsCount - inputArgsCount);
    } else {
        expectedStackOffset = initialStackOffset - (inputArgsCount - outputArgsCount);
    }

    if (expectedStackOffset != _controlBlock.stackOffset) {
        // todo: do we need a mechanism to rollback side effect of procedure?
        SetState(ProcessorState::StackCorrupted);
        return false;
    }

    return true;
}

bool ProcessorContext::PushStack(std::byte data)
{
    const bool success = _stack.TryPush(data);
    if (!success) {
        SetState(ProcessorState::StackOverflow);
    }
    return success;
}

std::pair<bool, std::byte> ProcessorContext::PopStack()
{
    const auto [success, data] = _stack.TryPop<std::byte>();
    if (!success) {
        SetState(ProcessorState::StackUnderflow);
    }
    return { success, data };
}

ProcessorControlBlockGuard ProcessorContext::MakeGuard()
{
    return { _controlBlock };
}
