#pragma once

#include "base_script.h"

class CommandLine;

class SetupScript : public BaseScript {
public:
    SetupScript(const CommandLine& commandLine);

    void Perform() override;

    common::Storage ExtractParameters();

private:
    const CommandLine& _commandLine;
    std::unique_ptr<common::Storage> _parameters;
};