#include "command_line.h"

namespace common {

CommandLine::CommandLine(int argc, char** argv)
    : _argc(argc)
    , _argv(argv)
{
}
std::optional<std::string_view> CommandLine::TryFindValue(const std::string_view key) const
{
    for (int i = 0; i < _argc; ++i) {
        if (_argv[i] != key) {
            continue;
        }
        if (i + 1 >= _argc) {
            continue;
        }
        return _argv[i + 1];
    }
    return {};
}
}