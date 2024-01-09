#include "brain_data.h"

BrainData::BrainData(std::span<std::byte> memory)
    : BrainDataBase(memory)
{
}

ConstBrainData::ConstBrainData(std::span<const std::byte> memory)
    : BrainDataBase(memory)
{
}
