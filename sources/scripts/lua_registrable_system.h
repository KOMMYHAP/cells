#pragma once

#include "registrar/registrable_system.h"

class LuaLogger;
class LuaSystem;
class LuaRegistrableSystem final : public common::RegistrableSystem {
public:
    struct Config {
        std::filesystem::path luaDirectory;
        std::optional<std::string_view> startupScript;
    };

    LuaRegistrableSystem();
    ~LuaRegistrableSystem() override;

    std::error_code InitializeSystem(ApplicationStorage& storage) override;
    void TerminateSystem() override;

    const LuaSystem& GetLuaSystem() { return *_luaSystem; }
    LuaSystem& ModifyLuaSystem() { return *_luaSystem; }

private:
    void MakeDefaultLuaLogger();

    LuaSystem* _luaSystem{nullptr};
    std::unique_ptr<LuaLogger> _defaultLogger;
};
