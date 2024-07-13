#include "virtual_machine.h"

#include "procedures/procedure_context.h"
#include "processor/processor.h"
#include "processor/processor_control_block.h"

static_assert(ProcessorStackSize >= std::max(ProcedureInputArgsCountLimit, ProcedureOutputArgsCountLimit),
    "Register overflow! Processor use registries to pass args to or obtain args from procedure");

static_assert(sizeof(ProcessorControlBlock::pendingProcedureId) == sizeof(std::underlying_type_t<PendingProcedureId>));
static_assert(sizeof(ProcessorControlBlock::state) == sizeof(std::underlying_type_t<ProcessorState>));

static_assert(std::is_trivial_v<ProcessorControlBlock>, "As part of memory view ProcessorControlBlock must be trivial");
static_assert(std::is_trivial_v<ProcessorInstruction>, "As part of memory view ProcessorInstruction must be trivial");
static_assert(std::is_trivial_v<ProcessorFlags>, "As part of memory view ProcessorFlags must be trivial");

ProcedureId VirtualMachine::RegisterProcedure(ProcedureBase* procedure, uint8_t inputArgs, uint8_t outputArgs)
{
    const auto info = ProcedureTableEntry { inputArgs, outputArgs, procedure };
    const ProcedureId id = _procedureTable.RegisterProcedure(info);
    return id;
}

void VirtualMachine::Run(ProcessorMemory memory, std::any procedureExternalContext /*= {}*/)
{
    const auto [controlBlockRead, controlBlock] = memory.TryAccess<ProcessorControlBlock>();
    ASSERT(controlBlockRead);

    const PendingProcedureId pendingProcedureId = AllocatePendingProcedureId();
    PendingProcedurePlaceholder& placeholder = GetPendingProcedureContext(pendingProcedureId);

    ProcessorContext::Params params {
        &_procedureTable,
        controlBlock,
        &placeholder,
        ProcedureExternalContext { std::move(procedureExternalContext) },
        memory
    };
    ProcessorContext context { std::move(params) };

    Processor processor;
    processor.SetDebugger(_debugger);
    processor.Execute(context);

    if (!context.HasPendingProcedure()) {
        FreePendingProcedureId(pendingProcedureId);
    }
}

ProcedureContext VirtualMachine::RestoreDeferredExecution(ProcessorMemory memory)
{
    const auto [controlBlockRead, controlBlock] = memory.TryAccess<ProcessorControlBlock>();
    ASSERT(controlBlockRead);
    static constexpr auto RawInvalidPendingProcedureId = static_cast<std::underlying_type_t<PendingProcedureId>>(PendingProcedureId::Invalid);
    const auto rawPendingProcedureId = std::exchange(controlBlock->pendingProcedureId, RawInvalidPendingProcedureId);
    const auto pendingProcedureId = static_cast<PendingProcedureId>(rawPendingProcedureId);
    ASSERT(pendingProcedureId != PendingProcedureId::Invalid);
    return ExtractProcedureContext(pendingProcedureId);
}

ProcedureContext VirtualMachine::ExtractProcedureContext(PendingProcedureId id)
{
    const size_t index { static_cast<std::underlying_type_t<PendingProcedureId>>(id) };
    PendingProcedurePlaceholder& placeholder = _pendingProcedures[index];
    const ProcedureContext context = placeholder.Extract();
    FreePendingProcedureId(id);
    return context;
}

PendingProcedureId VirtualMachine::AllocatePendingProcedureId()
{
    if (_freeIds.empty()) {
        ASSERT(_nextFreeId != PendingProcedureId::Invalid);
        _freeIds.push(_nextFreeId);
        _pendingProcedures.emplace_back();
        const auto rawNextFreeId = static_cast<std::underlying_type_t<PendingProcedureId>>(_nextFreeId) + 1;
        _nextFreeId = static_cast<PendingProcedureId>(rawNextFreeId);
    }
    const PendingProcedureId id = _freeIds.top();
    _freeIds.pop();
    return id;
}

PendingProcedurePlaceholder& VirtualMachine::GetPendingProcedureContext(PendingProcedureId id)
{
    const size_t index { static_cast<std::underlying_type_t<PendingProcedureId>>(id) };
    return _pendingProcedures[index];
}

void VirtualMachine::FreePendingProcedureId(const PendingProcedureId id)
{
    _freeIds.push(id);
}

void VirtualMachine::SetDebugger(ProcessorDebugger* debugger)
{
    _debugger = debugger;
}
