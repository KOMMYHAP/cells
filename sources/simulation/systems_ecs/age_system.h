#pragma once

#include "simulation/simulation_system.h"
#include "simulation/simulation_storage.h"

std::unique_ptr<SimulationSystem> RegisterAgeSystem(const SimulationStorage & storage);