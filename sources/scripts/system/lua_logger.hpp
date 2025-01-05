#pragma once

template <class... Args>
void LuaLogger::Error(std::format_string<Args...> format, Args&&... args)
{
    std::println(format, std::forward<Args>(args)...);
    if (_onError) {
        _onError();
    }
}