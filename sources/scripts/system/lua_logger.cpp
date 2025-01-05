#include "lua_logger.h"

void LuaLogger::BreakIfNeeded()
{
    std::fflush(stdout);
    ASSERT_FAIL("Lua error occurred!");
}