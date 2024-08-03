#include "processor_stack.h"

#include "processor_control_block.h"

ProcessorStack::ProcessorStack(const ProcessorControlBlock& controlBlock)
    : _buffer(controlBlock.stack)
    , _offset(controlBlock.stackOffset)
{
}

void ProcessorStack::CopyTo(ProcessorControlBlock& controlBlock) const
{
    controlBlock.stack = _buffer;
    controlBlock.stackOffset = _offset;
}

bool ProcessorStack::CanPush(uint8_t bytes) const
{
    return GetPushedBytesCount() + bytes <= GetBytesCapacity();
}

uint8_t ProcessorStack::GetPushedBytesCount() const
{
    return _offset;
}

uint8_t ProcessorStack::GetBytesCapacity() const
{
    return NarrowCast<uint8_t>(_buffer.size());
}

bool ProcessorStack::CanPop(uint8_t count) const
{
    return count <= GetPushedBytesCount();
}