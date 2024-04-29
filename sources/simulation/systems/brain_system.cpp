#include "brain_system.h"

#include "processor/processor_control_block.h"

BrainSystem::BrainSystem(uint32_t capacity)
    : _cells(capacity)
{
}

CellBrain& BrainSystem::Access(CellId id)
{
    const auto index = CellIdToInt(id);
    ASSERT(index <= _cells.size());
    return _cells[index];
}

const CellBrain& BrainSystem::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    ASSERT(index <= _cells.size());
    return _cells[index];
}

ProcessorMemory BrainSystem::AccessMemory(CellId id)
{
    CellBrain& cell = Access(id);
    return ProcessorMemory(std::span(cell.data));
}

ProcessorConstMemory BrainSystem::GetMemory(CellId id) const
{
    const CellBrain& cell = Get(id);
    return ProcessorConstMemory(std::span(cell.data));
}

void BrainSystem::DumpCellBrain(std::ostream& stream, CellId id)
{
    stream << std::format("Cell Id #{}\n", CellIdToInt(id));
    ProcessorConstMemory memory = GetMemory(id);
    const auto controlBlock = memory.Read<ProcessorControlBlock>();
    stream << "Control Block:\n";
    stream << std::format("state: {}, flags: {}", controlBlock.state, controlBlock.flags);
}
