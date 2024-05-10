#include "common_system.h"
#include "random.h"
#include "storage/stack_storage.h"

namespace common {

CommonSystem::CommonSystem(int argc, char** argv)
    : _argc(argc)
    , _argv(argv)
{
}

std::error_code CommonSystem::InitializeSystem(StackStorage& storage)
{
    common::InitRandom("42");
    storage.Store<CommandLine>(_argc, _argv);
    return std::error_code();
}

void CommonSystem::TerminateSystem()
{
    common::TermRandom();
}

}