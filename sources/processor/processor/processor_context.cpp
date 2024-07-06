#include "processor_context.h"

#include "flags.h"
#include "processor_state.h"
#include "procedures/procedure_context.h"

ProcessorContext::ProcessorContext(Params params)
    : _params(std::move(params))
    , _initialMemory(_params.memory)
    , _stack(_params.controlBlock->stack, _params.controlBlock->stackOffset)
{
    if (!SetCommandPointer(_params.controlBlock->nextCommand)) {
        ASSERT_FAIL("Invalid command pointer!");
    }
    ASSERT(*_params.stateWatcher, "Invalid state watcher!");
}

bool ProcessorContext::HasFlag(ProcessorFlags flag) const
{
    return common::HasFlag(_params.controlBlock->flags, flag);
}

void ProcessorContext::SetFlag(ProcessorFlags flag)
{
    common::SetFlag(_params.controlBlock->flags, flag);
}

void ProcessorContext::ResetFlag(ProcessorFlags flag)
{
    common::ResetFlag(_params.controlBlock->flags, flag);
}

bool ProcessorContext::IsState(ProcessorState state) const
{
    return _params.controlBlock->state == static_cast<uint8_t>(state);
}

void ProcessorContext::SetState(ProcessorState state)
{
    _params.controlBlock->state = static_cast<uint8_t>(state);
    (*_params.stateWatcher)(state, _params.externalContext);
}

bool ProcessorContext::SetCommandPointer(uint8_t nextCommand)
{
    if (!_initialMemory.HasBytes(nextCommand)) {
        SetState(ProcessorState::OutOfMemory);
        return false;
    }
    _params.controlBlock->nextCommand = nextCommand;
    _params.memory = ProcessorMemory { _initialMemory.MakeSpan(_params.controlBlock->nextCommand) };
    return true;
}

bool ProcessorContext::MoveCommandPointer(uint8_t offset)
{
    if (_params.controlBlock->nextCommand + offset <= _params.controlBlock->nextCommand) {
        SetState(ProcessorState::OutOfMemory);
        return false;
    }
    return SetCommandPointer(_params.controlBlock->nextCommand + offset);
}

bool ProcessorContext::WriteRegistry(uint8_t index, std::byte data)
{
    if (index >= _params.controlBlock->registry.size()) {
        SetState(ProcessorState::InvalidInstruction);
        return false;
    }
    _params.controlBlock->registry[index] = data;
    return true;
}

std::pair<bool, std::byte> ProcessorContext::ReadRegistry(uint8_t index)
{
    if (index >= _params.controlBlock->registry.size()) {
        SetState(ProcessorState::InvalidInstruction);
        return { false, std::byte {} };
    }
    return { true, _params.controlBlock->registry[index] };
}

bool ProcessorContext::StartProcedure(ProcedureId id)
{
    const ProcedureTableEntry* info = _params.procedureTable->FindProcedure(id);
    if (!info) {
        SetState(ProcessorState::UnknownProcedure);
        return false;
    }

    uint8_t inputArgsCount = info->inputArgsCount;
    uint8_t outputArgsCount = info->outputArgsCount;
    const uint8_t initialStackOffset = _params.controlBlock->stackOffset;

    if (initialStackOffset < inputArgsCount) {
        SetState(ProcessorState::ProcedureMissingInput);
        return false;
    }
    
    ASSERT(!_pendingProcedure.has_value());
    _pendingProcedure = id;

    ProcedureContext procedureContext { id, *this, _stack, inputArgsCount, outputArgsCount };
    info->procedure->Execute(procedureContext);
    return true;
}

bool ProcessorContext::CompleteProcedure(ProcedureId id, uint8_t ignoredInputArgs, uint8_t missingOutputArgs)
{
    if (!_pendingProcedure.has_value() || *_pendingProcedure != id) {
        SetState(ProcessorState::UnknownDelayedProcedure);
        return false;
    }
    if (!IsState(ProcessorState::Good)) {
        return false;
    }
    if (ignoredInputArgs != 0) {
        SetState(ProcessorState::ProcedureIgnoreInput);
        return false;
    }
    if (missingOutputArgs != 0) {
        SetState(ProcessorState::ProcedureMissingOutput);
        return false;
    }
    _pendingProcedure.reset();
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
    return { *_params.controlBlock };
}

void ProcessorContext::SetFlag(ProcessorFlags flag, bool value)
{
    value ? SetFlag(flag) : ResetFlag(flag);
}