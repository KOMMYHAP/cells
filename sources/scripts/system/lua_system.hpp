#pragma once
#include "sol/sol.hpp"

template <class... Args>
sol::function_result LuaSystem::RunScript(std::string_view scriptName, Args&&... args)
{
    sol::load_result* loadResult = FindScript(scriptName);
    if (!loadResult) {
        return {};
    }
    if (!loadResult->valid()) {
        return {};
    }
    sol::function_result r = loadResult->call(std::forward<Args>(args)...);
    if (!r.valid()) {
        _logger->Error("Failed to execute script: {}!", sol::error { r }.what());
        return {};
    }
    return r;
}