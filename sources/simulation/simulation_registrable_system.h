﻿#pragma once

#include "registrar/registrable_system.h"

class World;

class SimulationRegistrableSystem final : public common::RegistrableSystem {
public:
    SimulationRegistrableSystem();
    SimulationRegistrableSystem(const SimulationRegistrableSystem& other) = delete;
    SimulationRegistrableSystem(SimulationRegistrableSystem&& other) noexcept = delete;
    SimulationRegistrableSystem& operator=(const SimulationRegistrableSystem& other) = delete;
    SimulationRegistrableSystem& operator=(SimulationRegistrableSystem&& other) noexcept = delete;
    ~SimulationRegistrableSystem() override;
    
    std::error_code InitializeSystem(ApplicationStorage& storage) override;
    void TerminateSystem() override;

private:
    std::unique_ptr<World> _world;
};