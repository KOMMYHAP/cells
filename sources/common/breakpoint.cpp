#include "breakpoint.h"

namespace {

void SignalHandler(int signal)
{
    if (signal == SIGINT) {
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
        std::signal(SIGINT, &SignalHandler);
    } else {
        std::signal(SIGINT, SIG_DFL);
    }
}

}
