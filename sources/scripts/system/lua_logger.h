#pragma once

class LuaLogger {
public:
    template <class... Args>
    void Error(std::format_string<Args...> format, Args&&... args);

private:
    void BreakIfNeeded();
};

#include "lua_logger.hpp"