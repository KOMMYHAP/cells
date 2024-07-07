#include "crossover_algorithm.h"

#include "procedures/pending_procedure_id.h"
#include "procedures/procedure_context.h"
#include "processor/processor_control_block.h"
#include "processor/processor_memory.h"

CellBrain CrossoverAlgorithm::Combine(const CellBrain& parentLeft, const CellBrain& parentRight)
{
    ProcessorConstMemory memoryLeft { parentLeft.data };
    ASSERT(memoryLeft.HasBytes<ProcessorControlBlock>());

    memoryLeft.Move<ProcessorControlBlock>();
    auto blobLeft = memoryLeft.MakeSpan(0);

    ProcessorConstMemory memoryRight { parentRight.data };
    ASSERT(memoryRight.HasBytes<ProcessorControlBlock>());
    memoryRight.Move<ProcessorControlBlock>();
    auto blobRight = memoryRight.MakeSpan(0);

    const uint8_t point = _point;

    CellBrain brain {};
    ProcessorMemory memory { brain.data };
    ProcessorControlBlock controlBlock {
        static_cast<std::underlying_type_t<ProcessorState>>(ProcessorState::Good),
        static_cast<std::underlying_type_t<PendingProcedureId>>(PendingProcedureId::Invalid),
        0,
        0,
        {},
        0,
        {}
    };
    const bool writeSuccess = memory.TryWrite(controlBlock);
    ASSERT(writeSuccess);

    auto blobOut = memory.MakeSpan(0);
    std::memcpy(blobOut.data(), blobLeft.data(), point);
    std::memcpy(blobOut.data() + point, blobRight.data() + point, blobOut.size() - point);
    return brain;
}

CrossoverAlgorithm::CrossoverAlgorithm(uint8_t point)
    : _point(point)
{
}