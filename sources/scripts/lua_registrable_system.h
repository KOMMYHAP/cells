#pragma once

#include "registrar/registrable_system.h"

class LuaLogger;
class LuaSystem;
class LuaRegistrableSystem final : public common::RegistrableSystem {
public:
    LuaRegistrableSystem();
    ~LuaRegistrableSystem() override;

    std::error_code InitializeSystem(ApplicationStorage& storage) override;
    void TerminateSystem() override;

    const LuaSystem& GetLuaSystem() { return *_luaSystem; }
    LuaSystem& ModifyLuaSystem() { return *_luaSystem; }

private:
    void MakeDefaultLuaLogger();

    std::unique_ptr<LuaSystem> _luaSystem;
    std::unique_ptr<LuaLogger> _defaultLogger;
};
