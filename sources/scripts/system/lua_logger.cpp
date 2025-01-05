#include "lua_logger.h"

LuaLogger::LuaLogger(std::function<void()> onErrorCallback)
    : _onError(std::move(onErrorCallback))
{
}