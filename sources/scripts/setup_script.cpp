#include "setup_script.h"
#include "command_line.h"
#include "storage.h"

#include "world_render.h"
#include "world_render_parameters.h"

#include "world_widget_parameters.h"

#include "cell_factories/patrol_cell_factory.h"
#include "cell_factories/random_cell_factory.h"
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
#include "world_parameters.h"

static const std::string_view FontArgument = "--font";
static const std::string_view FragmentShaderArgument = "--fragment-shader";

const sf::Color Gray { 0xCCCCCCFF };
const uint16_t StatusMessageBufferLimit = 200;

SetupScript::SetupScript(const CommandLine& commandLine)
{
}

void SetupScript::Perform()
{
    common::Storage parameters;
    auto& worldWidgetParameters = parameters.Store<WorldWidgetParameters>();
    worldWidgetParameters.screenWidth = 800;
    worldWidgetParameters.screenHeight = 600;
    worldWidgetParameters.fieldOffset = 20;
    worldWidgetParameters.fieldWidth = worldWidgetParameters.screenWidth - 2 * worldWidgetParameters.fieldOffset;
    worldWidgetParameters.fieldHeight = worldWidgetParameters.screenHeight - 2 * worldWidgetParameters.fieldOffset;
    worldWidgetParameters.statusTextOffset = 5;
    worldWidgetParameters.statusTextSize = 10;
    worldWidgetParameters.cellPadding = 0;
    worldWidgetParameters.cellSize = 8;

    const uint32_t rowsCount = worldWidgetParameters.fieldHeight / (worldWidgetParameters.cellSize + worldWidgetParameters.cellPadding);
    const uint32_t columnsCount = worldWidgetParameters.fieldWidth / (worldWidgetParameters.cellSize + worldWidgetParameters.cellPadding);

    auto& worldRenderParameters = parameters.Store<WorldRenderParameters>();

    auto& simulationParameters = parameters.Store<SimulationParameters>();
    simulationParameters.targetSimulationTime = sf::milliseconds(15);
    simulationParameters.cellsCountPercentOfLimit = 20;

    auto& worldParameters = parameters.Store<WorldParameters>();

    common::Storage& systems = worldParameters.systems;
    auto& _idSystem = systems.Store<IdSystem>(rowsCount * columnsCount);
    auto& _brainSystem = systems.Store<BrainSystem>(_idSystem.GetCellsCountLimit());
    auto& _typeSystem = systems.Store<TypeSystem>(_idSystem.GetCellsCountLimit());
    auto& _positionSystem = systems.Store<PositionSystem>(rowsCount, columnsCount);
    auto& _graveyardSystem = systems.Store<GraveyardSystem>(_idSystem.GetCellsCountLimit(), _idSystem, _typeSystem, _positionSystem);
    auto& _healthSystem = systems.Store<HealthSystem>(_idSystem.GetCellsCountLimit(), _graveyardSystem);

    auto watcher = [](ProcessorState state) {
        //        if (state == ProcessorState::Good) {
        //            return;
        //        }
        //
        //        // Cell's brain has illegal instruction, make insult as punishment
        //        const CellId id = world->GetSystems().Get<SimulationVirtualMachine>().GetRunningCellId();
        //        world->ModifySystems().Modify<HealthSystem>().Set(id, CellHealth::Zero);
    };
    constexpr uint8_t systemInstructionPerStep = 8;
    SimulationVirtualMachine::Config simulationVmConfig {
        world->ModifySystems(),
        systemInstructionPerStep,
        std::move(watcher)
    };
    auto& _simulationVm = systems.Store<SimulationVirtualMachine>(MakeSimulationVmConfig(this));
    auto& _ageSystem = systems.Store<AgeSystem>(_idSystem.GetCellsCountLimit(), _healthSystem);
    auto& _spawner = systems.Store<Spawner>(_positionSystem, _typeSystem, _brainSystem, _healthSystem, _ageSystem, _idSystem);
    auto& _spawnSystem = systems.Store<SpawnSystem>(MakeSpawnSystemConfig(config.fullnessPercent));
    auto& _render = systems.Store<WorldRender>(MakeRenderConfig(config.cellSize, std::move(worldRenderParameters.shader)), _positionSystem, _idSystem, _typeSystem);
    auto& _selectionSystem = systems.Store<SelectionSystem>(_brainSystem, _idSystem, SelectionEpochTicks, BestCellSelectionSize);
    auto _patrolCellFactory = std::make_unique<PatrolCellFactory>(_simulationVm, 10);
    auto _randomCellFactory = std::make_unique<RandomCellFactory>(_simulationVm, std::optional<uint16_t>());

    _parameters = std::make_unique<common::Storage>(std::move(parameters));
}

common::Storage SetupScript::ExtractParameters()
{
    ASSERT(_parameters);
    common::Storage systems = std::move(*_parameters);
    _parameters.reset();
    return systems;
}
