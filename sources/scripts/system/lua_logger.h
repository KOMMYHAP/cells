#pragma once

class LuaLogger {
public:
    LuaLogger(std::function<void()> onErrorCallback);

    template <class... Args>
    void Error(std::format_string<Args...> format, Args&&... args);

private:
    void BreakIfNeeded();

    std::function<void()> _onError;
};

#include "lua_logger.hpp"