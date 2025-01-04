#pragma once

class LuaSystem {
public:
    LuaSystem();
    ~LuaSystem();

private:
    sol::state _luaState;
};