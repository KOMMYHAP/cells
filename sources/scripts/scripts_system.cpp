#include "scripts_system.h"
#include <world_parameters.h>

#include "command_line.h"
#include "setup_script.h"
#include "storage/stack_storage.h"
#include "ui_layout.h"
#include "world_parameters.h"

std::error_code ScriptSystem::InitializeSystem(common::StackStorage& storage)
{
    auto& commandLine = storage.Get<common::CommandLine>();
    SetupScript setupScript { commandLine };
    auto result = setupScript.Perform();
    if (!result) {
        return result;
    }

    auto setup = setupScript.ExtractParameters();

    WorldParameters worldParameters;
    worldParameters.systems = std::move(setup.systems);
    worldParameters.simulationScript = std::move(setup.simulationScript);

    for (auto&& [_, factory] : setup.factories) {
        worldParameters.factories.emplace_back(std::move(factory));
    }

    storage.Store<WorldParameters>(std::move(worldParameters));
    storage.Store<UiLayout>(setup.uiLayout);

    return std::error_code();
}
void ScriptSystem::TerminateSystem()
{
}
