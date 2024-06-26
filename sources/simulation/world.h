#pragma once

#include "registrar/registrable_system.h"
#include "simulation.h"
#include "world_parameters.h"

class World : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(common::StackStorage& storage) override;
    void TerminateSystem() override;

    void Update(sf::Time elapsedTime);

    const Simulation& GetSimulation() const { return *_simulation; }
    Simulation& ModifySimulation() { return *_simulation; }

    const common::StackStorage& GetSystems() const { return _parameters->systems; }

private:
    WorldParameters* _parameters { nullptr };
    std::unique_ptr<Simulation> _simulation;
};