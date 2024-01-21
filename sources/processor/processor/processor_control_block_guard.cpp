#include "processor_control_block_guard.h"

ProcessorControlBlockGuard::ProcessorControlBlockGuard(ProcessorControlBlock& controlBlock)
    : _controlBlock(controlBlock)
    , _initialControlBlock(_controlBlock)
    , _shouldRollback(true)
{
}
ProcessorControlBlockGuard::~ProcessorControlBlockGuard()
{
    if (_shouldRollback) {
        _controlBlock = _initialControlBlock;
    }
}

void ProcessorControlBlockGuard::Submit()
{
    _shouldRollback = false;
}