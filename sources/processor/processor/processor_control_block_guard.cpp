#include "processor_control_block_guard.h"

#include "processor_context.h"

ProcessorControlBlockGuard::ProcessorControlBlockGuard(ProcessorContext& context)
    : _context(&context)
    // , _initialControlBlock(context.GetControlBlock())
{
    ASSERT_FAIL("not implemented");
}
ProcessorControlBlockGuard::ProcessorControlBlockGuard(ProcessorControlBlockGuard&& other) noexcept
    : _shouldRollback(std::exchange(other._shouldRollback, false))
    , _context(other._context)
    , _initialControlBlock(other._initialControlBlock)
{
}

ProcessorControlBlockGuard& ProcessorControlBlockGuard::operator=(ProcessorControlBlockGuard&& other) noexcept
{
    if (this == &other) {
        return *this;
    }
    ProcessorControlBlockGuard temp(std::move(other));
    Swap(temp);
    return *this;
}

ProcessorControlBlockGuard::~ProcessorControlBlockGuard()
{
    if (_shouldRollback) {
        ASSERT_FAIL("not implemented");
        // _context->Rollback(_initialControlBlock);
    }
}

void ProcessorControlBlockGuard::Swap(ProcessorControlBlockGuard& other) noexcept
{
    std::swap(_shouldRollback, other._shouldRollback);
    std::swap(_context, other._context);
    std::swap(_initialControlBlock, other._initialControlBlock);
}

void ProcessorControlBlockGuard::Submit()
{
    _shouldRollback = false;
}