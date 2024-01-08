#include "brain_data.h"
#include "brain_view.h"
#include "cell.h"

BrainData::BrainData(std::span<std::byte> memory)
    : memory(memory)
{
}

void BrainData::Move(uint8_t offset)
{
    memory = memory.subspan(offset);
}

bool BrainData::HasBytes(uint8_t count) const
{
    return Size() >= count;
}
