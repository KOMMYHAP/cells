#pragma once

namespace common {

class CommandLine {
public:
    CommandLine(int argc, char** argv);

    std::optional<std::string_view> FindValue(std::string_view key) const;

private:
    int _argc { 0 };
    char** _argv { nullptr };
};

}