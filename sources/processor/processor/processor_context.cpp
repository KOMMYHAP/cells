#include "processor_context.h"

#include "flags.h"
#include "procedures/procedure_context.h"
#include "procedures/procedure_table.h"
#include "processor_state.h"

ProcessorContext::ProcessorContext(Params params)
    : _params(std::move(params))
    , _initialMemory(_params.memory)
{
    if (!SetCommandPointer(_params.controlBlock->nextCommand)) {
        ASSERT_FAIL("Invalid command pointer!");
    }
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
    return static_cast<std::underlying_type_t<ProcessorState>>(state) == _params.controlBlock->state;
}

ProcessorState ProcessorContext::GetState() const
{
    return static_cast<ProcessorState>(_params.controlBlock->state);
}

void ProcessorContext::SetState(ProcessorState state)
{
    _params.controlBlock->state = static_cast<std::underlying_type_t<ProcessorState>>(state);
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

bool ProcessorContext::HasPendingProcedure() const
{
    return IsState(ProcessorState::PendingProcedure);
}

void ProcessorContext::SetPendingProcedure(ProcedureId id)
{
    _params.controlBlock->pendingProcedureId = static_cast<std::underlying_type_t<ProcedureId>>(id);
}

ProcedureId ProcessorContext::GetPendingProcedure() const
{
    return static_cast<ProcedureId>(_params.controlBlock->pendingProcedureId);
}

bool ProcessorContext::PushStack(std::byte data)
{
    if (ProcessorStack stack { *_params.controlBlock }; stack.TryPush(data)) {
        stack.CopyTo(*_params.controlBlock);
        return true;
    }
    SetState(ProcessorState::StackOverflow);
    return false;
}

std::pair<bool, std::byte> ProcessorContext::PopStack()
{
    ProcessorStack stack { *_params.controlBlock };
    if (const auto [success, data] = stack.TryPop<std::byte>(); success) {
        stack.CopyTo(*_params.controlBlock);
        return { success, data };
    }
    SetState(ProcessorState::StackUnderflow);
    return { false, std::byte {} };
}

std::optional<ProcedureContext> ProcessorContext::MakeProcedureContext(ProcedureId id) const
{
    const ProcedureTableEntry* info = _params.procedureTable->FindProcedure(id);
    if (!info) {
        return {};
    }
    const ProcedureContext::ArgumentsStatus arguments { info->inputArgsCount, info->outputArgsCount };
    const ProcessorStack stack { *_params.controlBlock };
    return ProcedureContext { id, _params.userData, stack, arguments };
}

ProcedureBase& ProcessorContext::GetProcedure(ProcedureId id)
{
    const ProcedureTableEntry* info = _params.procedureTable->FindProcedure(id);
    ASSERT(info != nullptr);
    return *info->procedure;
}

void ProcessorContext::SetFlag(ProcessorFlags flag, bool value)
{
    value ? SetFlag(flag) : ResetFlag(flag);
}