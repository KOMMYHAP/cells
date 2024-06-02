#pragma once

#include "registrar/registrable_system.h"

namespace scripts {

class LuaSystemImpl;

class LuaSystem : public common::RegistrableSystem {
public:
    LuaSystem();
    ~LuaSystem() override;

    std::error_code InitializeSystem(common::StackStorage& storage) override;
    void TerminateSystem() override;

    const LuaSystemImpl& GetImpl() const { return *_impl; }
    LuaSystemImpl& ModifyImpl() { return *_impl; }

private:
    std::unique_ptr<LuaSystemImpl> _impl;
};

}