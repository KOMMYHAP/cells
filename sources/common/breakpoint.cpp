#include "breakpoint.h"

namespace {

extern "C" using SignalHandlerType = void(*)(int);

static SignalHandlerType _defaultSignalHandler;

void DebugBreakpointAssertHandler(int signal)
{
    if (signal != SIGABRT) {
        _defaultSignalHandler(signal);
        return;
    }
    common::Breakpoint();
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
        _defaultSignalHandler = std::signal(SIGABRT, &DebugBreakpointAssertHandler);
    } else {
        std::signal(SIGABRT, _defaultSignalHandler);
    }
}

}
