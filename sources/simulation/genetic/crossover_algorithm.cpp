#include "crossover_algorithm.h"
#include "processor/processor_control_block.h"
#include "processor/processor_memory.h"

CrossoverAlgorithm::CrossoverAlgorithm(uint8_t kPoints)
    : _kPoints(kPoints)
{
}
CellBrain CrossoverAlgorithm::Combine(const CellBrain& parentLeft, const CellBrain& parentRight)
{
    // todo: check, test, refactor

    ProcessorConstMemory memoryLeft { parentLeft.data };
    if (!memoryLeft.HasBytes<ProcessorControlBlock>()) {
        assert(false);
        return {};
    }
    memoryLeft.Move<ProcessorControlBlock>();
    auto blobLeft = memoryLeft.MakeSubSpan(0);

    ProcessorConstMemory memoryRight { parentRight.data };
    if (!memoryRight.HasBytes<ProcessorControlBlock>()) {
        assert(false);
        return {};
    }
    memoryRight.Move<ProcessorControlBlock>();
    auto blobRight = memoryRight.MakeSubSpan(0);

    const uint8_t point = 10;

    CellBrain brain {};
    ProcessorMemory memory { brain.data };
    ProcessorControlBlock controlBlock {
        static_cast<uint8_t>(ProcessorState::Good),
        0,
        0,
        {},
        0,
        {}
    };
    const bool writeSuccess = memory.TryWrite(controlBlock);
    if (!writeSuccess) {
        assert(false);
        return {};
    }
    auto blobOut = memory.MakeSubSpan(0);
    std::memcpy(blobOut.data(), blobLeft.data(), point);
    std::memcpy(blobOut.data() + point, blobRight.data(), blobOut.size() - point);
    return brain;
}
