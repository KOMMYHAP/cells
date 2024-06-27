#pragma once

#include "application_storage.h"
#include "registrar/registrable_system.h"

#include "cell_factories/random_cell_factory.h"
#include "simulation.h"
#include "simulation_ecs.h"
#include "systems/position_system.h"
#include "systems_ecs/render_system.h"

class World final : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(ApplicationStorage& storage) override;
    void TerminateSystem() override;

    void Update(sf::Time elapsedTime);

    const EcsWorld& GetEcsWorld() const { return *_ecsWorld; }
    EcsWorld& ModifyEcsWorld() { return *_ecsWorld; }

    const Simulation& GetSimulation() const { return *_simulation; }
    Simulation& ModifySimulation() { return *_simulation; }

    sf::Vector2u GetWorldSize() const { return _worldSize; }

private:
    std::unique_ptr<EcsWorld> _ecsWorld;
    std::unique_ptr<Simulation> _simulation;
    std::vector<std::unique_ptr<SimulationSystem>> _simulationSystems;
    std::unique_ptr<PositionSystem> _currentPositions;
    std::unique_ptr<PositionSystem> _nextPositions;
    std::unique_ptr<ICellFactory> _cellFactory;
    sf::Vector2u _worldSize;
};