#include "processor_context.h"

#include "flags.h"
#include "processor_state.h"

ProcessorContext::ProcessorContext(const ProcedureTable& procedureTable, ProcessorControlBlock& controlBlock, const Memory& memory)
    : _procedureTable(procedureTable)
    , _controlBlock(controlBlock)
    , _memory(memory)
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

void ProcessorContext::SetStateWatcher(std::function<void(ProcessorState)> watcher)
{
    _watcher = std::move(watcher);
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

ConstMemory ProcessorContext::GetMemory() const
{
    return _memory.MakeSubSpan(_controlBlock.nextCommand);
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
    const ProcedureInfo* info = _procedureTable.FindProcedure(id);
    if (!info) {
        SetState(ProcessorState::UnknownProcedure);
        return false;
    }

    const uint8_t argsCount = info->inputArgsCount + info->outputArgsCount;
    if (argsCount >= _controlBlock.registry.size()) {
        SetState(ProcessorState::InvalidCommand);
        return false;
    }

    Memory registryMemory { std::span(_controlBlock.registry.begin(), _controlBlock.registry.begin() + argsCount) };
    ProcedureContext procedureContext { *this, registryMemory };
    info->procedure->Execute(procedureContext);
    return true;
}

template <class... Ts>
std::tuple<bool, Ts...> ProcessorContext::TryReadMemory()
{
    const auto result = GetMemory().TryRead<Ts...>();
    const bool success = std::get<0>(result);
    if (!success) {
        SetState(ProcessorState::MemoryCorrupted);
    }
    return result;
}
