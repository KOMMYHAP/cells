#pragma once

#include "base_script.h"
#include "components/cell_age.h"
#include "drawable.h"
#include "setup_script_errors.h"
#include "storage.h"

class Drawable;
class Updatable;
class Simulation;
class SimulationScript;

class SetupScript final : public BaseScript {
public:
    SetupScript(const common::CommandLine& commandLine);
    ~SetupScript();

    std::expected<void, std::error_code> Perform() override;

    struct Parameters {
        common::Storage systems;
        std::unique_ptr<SimulationScript> simulationScript;
        std::unique_ptr<Simulation> simulation;
    };
    Parameters ExtractParameters();

private:
    struct Config;

    Config MakeConfig();
    std::expected<common::Storage, std::error_code> MakeSystems(const Config& config);
    void SetupSystems(const common::Storage& system, const Config& config);

    const common::CommandLine& _commandLine;
    std::unique_ptr<Parameters> _parameters;
};