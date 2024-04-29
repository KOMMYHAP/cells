#include "processor_context.h"

#include "flags.h"
#include "processor_state.h"

ProcessorContext::ProcessorContext(const ProcedureTable& procedureTable, const ProcessorStateWatcher& stateWatcher, ProcessorControlBlock& controlBlock, const ProcessorMemory& memory)
    : _procedureTable(procedureTable)
    , _controlBlock(controlBlock)
    , _initialMemory(memory)
    , _memory(_initialMemory)
    , _watcher(stateWatcher)
    , _stack(_controlBlock.stack, _controlBlock.stackOffset)
{
    if (!SetCommandPointer(_controlBlock.nextCommand)) {
        UNREACHABLE("Invalid command pointer!");
    }
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
    if (!_initialMemory.HasBytes(nextCommand)) {
        SetState(ProcessorState::OutOfMemory);
        return false;
    }
    _controlBlock.nextCommand = nextCommand;
    _memory = ProcessorMemory { _initialMemory.MakeSpan(_controlBlock.nextCommand) };
    return true;
}

bool ProcessorContext::MoveCommandPointer(uint8_t offset)
{
    if (_controlBlock.nextCommand + offset <= _controlBlock.nextCommand) {
        SetState(ProcessorState::OutOfMemory);
        return false;
    }
    return SetCommandPointer(_controlBlock.nextCommand + offset);
}

bool ProcessorContext::WriteRegistry(uint8_t index, std::byte data)
{
    if (index >= _controlBlock.registry.size()) {
        SetState(ProcessorState::InvalidInstruction);
        return false;
    }
    _controlBlock.registry[index] = data;
    return true;
}

std::pair<bool, std::byte> ProcessorContext::ReadRegistry(uint8_t index)
{
    if (index >= _controlBlock.registry.size()) {
        SetState(ProcessorState::InvalidInstruction);
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

    uint8_t inputArgsCount = info->inputArgsCount;
    uint8_t outputArgsCount = info->outputArgsCount;
    const uint8_t initialStackOffset = _controlBlock.stackOffset;

    if (initialStackOffset < inputArgsCount) {
        SetState(ProcessorState::ProcedureMissingInput);
        return false;
    }

    ProcedureContext procedureContext { *this, _stack, inputArgsCount, outputArgsCount };
    info->procedure->Execute(procedureContext);
    if (!IsState(ProcessorState::Good)) {
        return false;
    }
    if (inputArgsCount != 0) {
        SetState(ProcessorState::ProcedureIgnoreInput);
        return false;
    }
    if (outputArgsCount != 0) {
        SetState(ProcessorState::ProcedureMissingOutput);
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

void ProcessorContext::SetFlag(ProcessorFlags flag, bool value)
{
    value ? SetFlag(flag) : ResetFlag(flag);
}
