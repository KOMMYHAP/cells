#include "setup_script.h"
#include "command_line.h"
#include "storage.h"

#include "ui_layout.h"
#include "world_render.h"

#include "cell_factories/patrol_cell_factory.h"
#include "cell_factories/random_cell_factory.h"
#include "setup_script_errors.h"
#include "simulation_parameters.h"
#include "systems/age_system.h"
#include "systems/brain_system.h"
#include "systems/graveyard_system.h"
#include "systems/health_system.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/selection_system.h"
#include "systems/simulation_virtual_machine.h"
#include "systems/spawn_system.h"
#include "systems/spawner.h"
#include "systems/type_system.h"

#include "simulation.h"
#include "simulation_script.h"
#include "updatable.h"

static const std::string_view FontArgument = "--font";
static const std::string_view FragmentShaderArgument = "--fragment-shader";

const sf::Color Gray { 0xCCCCCCFF };
const uint16_t StatusMessageBufferLimit = 200;

struct SetupScript::Config {
    // common
    uint16_t rowsCount { 0 };
    uint16_t columnsCount { 0 };

    // simulation
    sf::Time targetSimulationTime;

    // virtual machine
    uint8_t systemInstructionPerStep { 0 };

    // spawner
    float fullnessPercent { 0.0f };

    // selection
    CellAge limitCellAge { CellAge::Zero };
    uint16_t bestCellSelectionSize { 0 };
    uint16_t selectionEpochTicks { 0 };

    // render
    std::vector<sf::Color> colors;
};

SetupScript::SetupScript(const common::CommandLine& commandLine)
    : _commandLine(commandLine)
{
}

SetupScript::~SetupScript() = default;

std::expected<void, std::error_code> SetupScript::Perform()
{
    const Config config = MakeConfig();
    const UiLayout uiLayout = MakeUiLayout();

    auto mbSystems = MakeSystems(config, uiLayout);
    if (!mbSystems) {
        return std::unexpected { mbSystems.error() };
    }

    SetupSystems(mbSystems.value(), config);

    auto simulationScript = std::make_unique<SimulationScript>(mbSystems.value());
    SimulationParameters simulationParameters;
    simulationParameters.selectionEpochTicks = config.selectionEpochTicks;
    simulationParameters.bestCellSelectionSize = config.bestCellSelectionSize;
    simulationParameters.spawnPolicy = SimulationParameters::SpawnPolicy::Random;
    simulationParameters.limitCellAge = config.limitCellAge;
    simulationScript->SetParameters(simulationParameters);

    auto simulation = std::make_unique<Simulation>(*simulationScript);
    simulation->SetAutoMode(config.targetSimulationTime);

    _parameters = std::make_unique<Parameters>();
    _parameters->systems = std::move(mbSystems.value());
    _parameters->simulationScript = std::move(simulationScript);
    _parameters->simulation = std::move(simulation);
    _parameters->uiLayout = std::make_unique<UiLayout>(uiLayout);
    return {};
}

SetupScript::Config SetupScript::MakeConfig()
{
    SetupScript::Config config;
    // window
    config.rowsCount = 100;
    config.columnsCount = 50;

    // simulation
    config.targetSimulationTime = sf::milliseconds(15);

    // virtual machine
    config.systemInstructionPerStep = 8;

    // spawner
    config.fullnessPercent = 0.2f;

    // selection
    config.limitCellAge = CellAge { 100 };
    config.bestCellSelectionSize = 100;
    config.selectionEpochTicks = 100;

    config.colors = { sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White };
    return {};
}

SetupScript::Parameters SetupScript::ExtractParameters()
{
    ASSERT(_parameters);
    Parameters parameters = std::move(*_parameters);
    return parameters;
}

std::expected<common::Storage, std::error_code> SetupScript::MakeSystems(const Config& config, const UiLayout& uiLayout)
{
    auto mbFontPath = _commandLine.FindValue(FontArgument);
    if (!mbFontPath.has_value()) {
        std::cerr << std::format("Please specify filepath to font file using {} $path", FontArgument) << std::endl;
        return std::unexpected { make_error_code(SetupScriptErrors::MissingArgumentFont) };
    }

    sf::Font defaultFont;
    if (!defaultFont.loadFromFile(std::string { *mbFontPath })) {
        return std::unexpected { make_error_code(SetupScriptErrors::InvalidFont) };
    }

    auto mbFragmentShaderPath = _commandLine.FindValue(FragmentShaderArgument);
    if (!mbFragmentShaderPath.has_value()) {
        std::cerr << std::format("Please specify filepath to fragment shader using {} $path", FragmentShaderArgument) << std::endl;
        return std::unexpected { make_error_code(SetupScriptErrors::MissingArgumentShader) };
    }

    auto shader = std::make_unique<sf::Shader>();
    if (!shader->loadFromFile(std::string { *mbFragmentShaderPath }, sf::Shader::Fragment)) {
        return std::unexpected { make_error_code(SetupScriptErrors::InvalidShader) };
    }

    common::Storage systems;
    auto& idSystem = systems.Store<IdSystem>(config.rowsCount * config.columnsCount);
    auto& brainSystem = systems.Store<BrainSystem>(idSystem.GetCellsCountLimit());
    auto& typeSystem = systems.Store<TypeSystem>(idSystem.GetCellsCountLimit());
    auto& positionSystem = systems.Store<PositionSystem>(config.rowsCount, config.columnsCount);
    auto& graveyardSystem = systems.Store<GraveyardSystem>(idSystem.GetCellsCountLimit(), idSystem, typeSystem, positionSystem);
    auto& healthSystem = systems.Store<HealthSystem>(idSystem.GetCellsCountLimit(), graveyardSystem);
    /*auto& simulationVm =*/systems.Store<SimulationVirtualMachine>(brainSystem);
    auto& ageSystem = systems.Store<AgeSystem>(idSystem.GetCellsCountLimit(), healthSystem);
    auto& spawner = systems.Store<Spawner>(positionSystem, typeSystem, brainSystem, healthSystem, ageSystem, idSystem);
    /*auto& spawnSystem =*/systems.Store<SpawnSystem>(positionSystem, idSystem, spawner);

    WorldRender::Config worldRenderConfig {
        std::move(shader),
        config.colors,
        sf::Vector2u { uiLayout.fieldWidth, uiLayout.fieldHeight },
        positionSystem,
        idSystem,
        typeSystem
    };
    /*auto& render =*/systems.Store<WorldRender>(std::move(worldRenderConfig));
    /*auto& selectionSystem =*/systems.Store<SelectionSystem>(brainSystem, idSystem, config.selectionEpochTicks, config.bestCellSelectionSize);
    return systems;
}

void SetupScript::SetupSystems(const common::Storage& system, const Config& config)
{
    auto& _simulationVm = system.Modify<SimulationVirtualMachine>();
    auto& _healthSystem = system.Modify<HealthSystem>();
    auto& _spawnSystem = system.Modify<SpawnSystem>();
    auto& _idSystem = system.Modify<IdSystem>();

    _simulationVm.SetInstructionsPerStep(config.systemInstructionPerStep);
    auto watcher = [simulationVm = &_simulationVm, healthSystem = &_healthSystem](ProcessorState state) {
        if (state == ProcessorState::Good) {
            return;
        }
        // Cell's brain has illegal instruction, make insult as punishment
        const CellId id = simulationVm->GetRunningCellId();
        healthSystem->Set(id, CellHealth::Zero);
    };
    _simulationVm.SetWatcher(std::move(watcher));

    const auto targetPopulationSize = static_cast<uint32_t>(round(config.fullnessPercent * static_cast<float>(_idSystem.GetCellsCountLimit())));
    _spawnSystem.SetSpawnLimit(targetPopulationSize);

    const uint8_t moveCommandCount = 10;
    auto _patrolCellFactory = std::make_unique<PatrolCellFactory>(_simulationVm, moveCommandCount);
    auto _randomCellFactory = std::make_unique<RandomCellFactory>(_simulationVm, std::optional<uint16_t>());
}

UiLayout SetupScript::MakeUiLayout()
{
    UiLayout layout;
    layout.screenWidth = 800;
    layout.screenHeight = 600;
    layout.fieldOffset = 20;
    layout.fieldWidth = layout.screenWidth - 2 * layout.fieldOffset;
    layout.fieldHeight = layout.screenHeight - 2 * layout.fieldOffset;
    layout.statusTextOffset = 5;
    layout.statusTextSize = 10;
    layout.cellPadding = 0;

    //    ASSERT(StatusTextOffset * 2 + StatusTextSize <= FieldOffset);
    //    ASSERT(layout.fieldWidth % (CellSize + CellPadding) == 0);
    //    ASSERT(layout.fieldHeight % (CellSize + CellPadding) == 0);

    return layout;
}
