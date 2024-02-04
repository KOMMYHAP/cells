#include "random_cell_factory.h"

#include "processor/processor_control_block.h"
#include "processor/processor_memory.h"
#include "random.h"

RandomCellFactory::RandomCellFactory(std::optional<uint16_t> limitBrainSize)
    : _brainSize(limitBrainSize.value_or(CellBrainLimit - sizeof(ProcessorControlBlock)))
{
}

auto RandomCellFactory::Make() -> Result
{
    Result result;
    result.status = Status::FailedToCreate;

    ProcessorMemory memory { result.brain.data };
    if (!InitMemory(memory)) {
        return result;
    }
    if (!memory.HasBytes(_brainSize)) {
        return result;
    }

    std::span<std::byte> rawBrain = memory.MakeSpan();
    std::byte* begin = rawBrain.data();
    std::byte* end = rawBrain.data() + rawBrain.size();
    std::byte* it = begin;

    {
        using Step = uint64_t;
        std::uniform_int_distribution<Step> distribution {};
        for (; it + sizeof(Step) <= end; it += sizeof(Step)) {
            const Step value = distribution(common::GetRandomEngine());
            void* destination = it;
            const void* source = &value;
            std::memcpy(destination, source, sizeof(Step));
        }
    }
    {
        using RandomUnit = uint16_t; // minimal available in uniform_int_distribution
        using Step = uint8_t;
        std::uniform_int_distribution<RandomUnit> distribution {};
        for (; it + sizeof(Step) <= end; ++it) {
            const Step value = distribution(common::GetRandomEngine()) % sizeof(Step);
            void* destination = it;
            const void* source = &value;
            std::memcpy(destination, source, sizeof(Step));
        }
    }

    result.status = Status::Success;
    return result;
}

bool RandomCellFactory::InitMemory(ProcessorMemory& memory)
{
    // todo: extract it
    ProcessorControlBlock controlBlock {
        static_cast<uint8_t>(ProcessorState::Good),
        static_cast<uint8_t>(0),
        0,
        {},
        0,
        {}
    };
    return memory.TryWrite(controlBlock);
}
