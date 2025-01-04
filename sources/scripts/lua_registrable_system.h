#pragma once

#include "registrar/registrable_system.h"

class LuaSystem;
class LuaRegistrableSystem final : public common::RegistrableSystem {
public:
    LuaRegistrableSystem();
    ~LuaRegistrableSystem() override;

    std::error_code InitializeSystem(ApplicationStorage& storage) override;
    void TerminateSystem() override;

private:
    std::unique_ptr<LuaSystem> _luaSystem;
};
