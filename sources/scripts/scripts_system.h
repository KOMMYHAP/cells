#pragma once

#include "application_storage.h"
#include "registrar/registrable_system.h"
#include "simulation_system.h"

class ScriptSystem : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(ApplicationStorage& storage) override;
    void TerminateSystem() override;

private:
    std::vector<std::unique_ptr<SimulationSystem>> _systems;
};