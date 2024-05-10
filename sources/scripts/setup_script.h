#pragma once

#include "base_script.h"
#include "components/cell_age.h"
#include "drawable.h"
#include "setup_script_errors.h"
#include "simulation_script.h"
#include "storage/stack_storage.h"
#include "ui_layout.h"

class Drawable;
class Updatable;
class Simulation;
class SimulationScript;
class UiLayout;

class SetupScript final : public BaseScript {
public:
    SetupScript(const common::CommandLine& commandLine);
    ~SetupScript();

    std::error_code Perform() override;

    struct Parameters {
        common::StackStorage systems;
        std::map<SpawnPolicy, std::unique_ptr<ICellFactory>> factories;
        UiLayout uiLayout;
        std::unique_ptr<SimulationScript> simulationScript;
    };
    Parameters ExtractParameters();

private:
    struct Config;

    Config MakeConfig();
    UiLayout MakeUiLayout();
    std::expected<common::Storage, std::error_code> MakeSystems(const Config& config, const UiLayout& uiLayout);
    std::map<SpawnPolicy, std::unique_ptr<ICellFactory>> MakeSpawnFactories(const common::Storage& systems);
    void SetupSystems(const common::Storage& system, const Config& config);

    const common::CommandLine& _commandLine;
    std::unique_ptr<Parameters> _parameters;
};