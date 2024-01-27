#pragma once

#include "systems/brain_system.h"
#include "systems/graveyard_system.h"
#include "systems/health_system.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/simulation_virtual_machine.h"
#include "systems/spawn_system.h"
#include "systems/type_system.h"
#include "world_interface.h"
#include "world_render.h"

class WorldWhite final : public WorldInterface {
public:
    struct Config {
        uint32_t width;
        uint32_t height;
        uint32_t cellSize;
        uint8_t fullnessPercent;
        std::unique_ptr<sf::Shader> shader;
    };
    WorldWhite(Config&& config);

    void Tick() override;
    void Render(sf::RenderTarget& target, sf::RenderStates states);

    uint32_t GetCellsCount() const { return idSystem.GetCellsCount(); }

private:
    static WorldRender::Config MakeRenderConfig(uint32_t cellSize, std::unique_ptr<sf::Shader> shader);
    static SimulationVirtualMachine::Config MakeSimulationVmConfig(WorldWhite* world);

    void RegisterProcedures();
    void MakeTestField(uint8_t fullnessPercent);

    IdSystem idSystem;
    BrainSystem brainSystem;
    TypeSystem typeSystem;
    PositionSystem positionSystem;
    SpawnSystem cellFactory;
    GraveyardSystem graveyardSystem;
    HealthSystem healthSystem;
    SimulationVirtualMachine simulationVm;
    WorldRender _render;
};