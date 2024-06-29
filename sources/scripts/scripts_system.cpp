#include "scripts_system.h"

#include "simulation_ecs_system.h"
#include "systems_ecs/movement_system.h"
#include "world.h"

#include <entt/entity/registry.hpp>

std::error_code ScriptSystem::InitializeSystem(ApplicationStorage& storage)
{
    
    // auto& commandLine = storage.Get<common::CommandLine>();
    // SetupScript setupScript { commandLine };
    // const auto error = setupScript.Perform();
    // if (error) {
    //     return error;
    // }

    // auto& worldParameters = storage.Store<WorldParameters>();
    // auto setup = setupScript.ExtractParameters();
    // worldParameters.systems = std::move(setup.systems);

    // std::map<SpawnPolicy, ICellFactory*> weakFactories;
    // for (auto&& [policy, factory] : setup.factories) {
    //     weakFactories[policy] = factory.get();
    //     worldParameters.factories.emplace_back(std::move(factory));
    // }

    // auto simulationScript = std::make_unique<SimulationScript>(worldParameters.systems, std::move(weakFactories));
    // simulationScript->SetParameters(setup.initialSimulationParameters);
    // worldParameters.simulationScript = std::move(simulationScript);

    // storage.Store<UiLayout>(setup.uiLayout);
    // storage.Store<SimulationParameters>(setup.initialSimulationParameters);

    return {};
}

void ScriptSystem::TerminateSystem()
{
}
