#pragma once

#include "command_line.h"
#include "registrar/registrable_system.h"

namespace common {
class CommonSystem : public RegistrableSystem {
public:
    CommonSystem(int argc, char** argv);

    std::error_code InitializeSystem(StackStorage& storage) override;
    void TerminateSystem() override;

private:
    int _argc { 0 };
    char** _argv { nullptr };
};
}