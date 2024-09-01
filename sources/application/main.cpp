#include "basic_defines.h"

#include "registrar/registrar.h"

#include "simulation_registrable_system.h"
#include "ui_registrable_system.h"


int main(int /*argc*/, char** /*argv*/)
{
    common::Registrar registrar;
    registrar.Register<SimulationRegistrableSystem>();
    const UiRegistrableSystem& uiSystem = registrar.Register<UiRegistrableSystem>();

    if (const std::error_code error = registrar.RunInit()) {
        std::cerr << std::format("Failed to initialize systems: {}", error.message()) << '\n';
        return -1;
    }

    uiSystem.GetUiApplicationInterface()->ApplicationRunMainLoop();

    registrar.RunTerm();
    return 0;
}
