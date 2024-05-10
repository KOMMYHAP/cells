#include "setup_script.h"
#include "command_line.h"
#include "storage/storage.h"

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
    auto systems = std::make_unique<common::Storage>(std::move(mbSystems.value()));
    SetupSystems(*systems, config);
    auto factories = MakeSpawnFactories(*systems);
    std::map<SpawnPolicy , ICellFactory*> weakFactories;
    for (auto&&[policy, factory] : factories) {
        weakFactories[policy] = factory.get();
    }

    auto simulationScript = std::make_unique<SimulationScript>(*systems, std::move(weakFactories));
    SimulationParameters simulationParameters;
    simulationParameters.selectionEpochTicks = config.selectionEpochTicks;
    simulationParameters.bestCellSelectionSize = config.bestCellSelectionSize;
    simulationParameters.spawnPolicy = SpawnPolicy::Random;
    simulationParameters.limitCellAge = config.limitCellAge;
    simulationScript->SetParameters(simulationParameters);

    auto simulation = std::make_unique<Simulation>(*simulationScript);
    simulation->SetAutoMode(config.targetSimulationTime);

    _parameters = std::make_unique<Parameters>();
    _parameters->factories = std::move(factories);
    _parameters->systems = std::move(systems);
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

    config.colors = { sf::Color::Yellow, sf::Color::White, sf::Color::White, sf::Color::White };
    return config;
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
        return std::unexpected { make_error_code(SetupScriptErrors::MissingArgumentFont) };
    }

    sf::Font defaultFont;
    if (!defaultFont.loadFromFile(std::string { *mbFontPath })) {
        return std::unexpected { make_error_code(SetupScriptErrors::InvalidFont) };
    }

    auto mbFragmentShaderPath = _commandLine.FindValue(FragmentShaderArgument);
    if (!mbFragmentShaderPath.has_value()) {
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
    return std::move(systems);
}

void SetupScript::SetupSystems(const common::Storage& system, const Config& config)
{
    auto& simulationVm = system.Modify<SimulationVirtualMachine>();
    auto& healthSystem = system.Modify<HealthSystem>();
    auto& spawnSystem = system.Modify<SpawnSystem>();
    auto& idSystem = system.Modify<IdSystem>();

    simulationVm.SetInstructionsPerStep(config.systemInstructionPerStep);
    auto watcher = [&](ProcessorState state) {
        if (state == ProcessorState::Good) {
            return;
        }
        // Cell's brain has illegal instruction, make insult as punishment
        const CellId id = simulationVm.GetRunningCellId();
        healthSystem.Set(id, CellHealth::Zero);
    };
    simulationVm.SetWatcher(std::move(watcher));

    const auto targetPopulationSize = static_cast<uint32_t>(round(config.fullnessPercent * static_cast<float>(idSystem.GetCellsCountLimit())));
    spawnSystem.SetSpawnLimit(targetPopulationSize);
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
std::map<SpawnPolicy, std::unique_ptr<ICellFactory>> SetupScript::MakeSpawnFactories(const common::Storage& systems)
{
    std::map<SpawnPolicy, std::unique_ptr<ICellFactory>> factories;

    auto& simulationVm = systems.Modify<SimulationVirtualMachine>();

    const uint8_t moveCommandCount = 10;
    auto patrolCellFactory = std::make_unique<PatrolCellFactory>(simulationVm, moveCommandCount);
    auto randomCellFactory = std::make_unique<RandomCellFactory>(simulationVm, std::optional<uint16_t>());

    factories.emplace(SpawnPolicy::Random, std::move(randomCellFactory));
    factories.emplace(SpawnPolicy::Patrol, std::move(patrolCellFactory));
    return factories;
}
