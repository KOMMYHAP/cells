#include "command_line.h"
#include "registrar/registrar.h"

#include "main_loop.h"
#include "simulation_registrable_system.h"
#include "ui_system.h"

class StubCmdLine : public common::RegistrableSystem {
public:
    StubCmdLine(int argc, char** argv)
        : argc(argc)
        , argv(argv)
    {
    }

    std::error_code InitializeSystem(ApplicationStorage& storage) override
    {
        storage.Store<common::CommandLine>(argc, argv);
        return {};
    }
    void TerminateSystem() override { }

private:
    int argc;
    char** argv;
};

int main(int argc, char** argv)
{
    common::Registrar registrar;
    registrar.Register<StubCmdLine>(argc, argv);
    registrar.Register<SimulationRegistrableSystem>();
    registrar.Register<UiSystem>();
    auto& mainLoop = registrar.Register<MainLoop>();

    if (const std::error_code error = registrar.RunInit()) {
        std::cerr << std::format("Failed to initialize systems: {}", error.message()) << '\n';
        return -1;
    }

    mainLoop.Run();

    registrar.RunTerm();
    return 0;
}
