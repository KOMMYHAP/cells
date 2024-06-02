#pragma once

#include "base_script.h"
#include "components/cell_age.h"
#include "setup_script_errors.h"
#include "simulation_script.h"
#include "storage/stack_storage.h"
#include "ui_layout.h"

class Simulation;
class SimulationScript;
struct UiLayout;

class SetupScript final : public BaseScript {
public:
    SetupScript(const common::CommandLine& commandLine);
    ~SetupScript() override;

    std::error_code Perform() override;

    struct Parameters {
        common::StackStorage systems;
        std::map<SpawnPolicy, std::unique_ptr<ICellFactory>> factories;
        UiLayout uiLayout;
        SimulationParameters initialSimulationParameters;
    };
    Parameters ExtractParameters();

private:
    struct Config;

    Config MakeConfig();
    SimulationParameters MakeSimulationParams();
    UiLayout MakeUiLayout();
    common::StackStorage MakeSystems(const Config& config);
    std::map<SpawnPolicy, std::unique_ptr<ICellFactory>> MakeSpawnFactories(const common::StackStorage& systems);
    void SetupSystems(const common::StackStorage& system, const Config& config);

    const common::CommandLine& _commandLine;
    std::unique_ptr<Parameters> _parameters;
};