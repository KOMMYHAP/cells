#pragma once

#include "registrar/registrable_system.h"

class ScriptSystem : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(common::StackStorage& storage) override;
    void TerminateSystem() override;

private:
};