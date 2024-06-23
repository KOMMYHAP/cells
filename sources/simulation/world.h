#pragma once

#include "registrar/registrable_system.h"

#include "simulation.h"
#include "simulation_ecs.h"

class World final : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(common::StackStorage& storage) override;
    void TerminateSystem() override;

    void Update(sf::Time elapsedTime);

    const EcsWorld& GetEcsWorld() const { return *_ecsWorld; }
    EcsWorld& ModifyEcsWorld() { return *_ecsWorld; }

    const Simulation& GetSimulation() const { return *_simulation; }
    Simulation& ModifySimulation() { return *_simulation; }

private:
    std::unique_ptr<EcsWorld> _ecsWorld;
    std::unique_ptr<Simulation> _simulation;
};