#pragma once

#include "registrar/registrable_system.h"
#include "simulation.h"
#include "systems/system.h"
#include "systems/system_registry.h"
#include "world_parameters.h"

class World : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(common::StackStorage& storage) override;
    void TerminateSystem() override;

    void Update(sf::Time elapsedTime);

    const Simulation& GetSimulation() const { return *_simulation; }
    Simulation& ModifySimulation() { return *_simulation; }

    //    const common::StackStorage& GetSystems() const { return _parameters->systems; }
    const SystemRegistry& GetSystems() const { return *_systems; }
    SystemRegistry& ModifySystems() { return *_systems; }

private:
    WorldParameters* _parameters { nullptr };
    std::unique_ptr<Simulation> _simulation;
    std::unique_ptr<SystemRegistry> _systems;
};