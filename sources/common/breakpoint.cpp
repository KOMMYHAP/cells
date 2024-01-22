#include "breakpoint.h"

namespace {

void SignalHandler(int signal)
{
    if (signal == SIGABRT) {
        common::Breakpoint();
    }
    std::quick_exit(EXIT_FAILURE);
}

}

namespace common {

void Breakpoint()
{
    __debugbreak();
}

void EnableBreakpointOnAssert(bool value)
{
    if (value) {
        std::signal(SIGABRT, &SignalHandler);
    } else {
        std::signal(SIGABRT, SIG_DFL);
    }
}

}
