#pragma once
#include "components/cell_brain.h"
#include "systems_ecs/simulation_virtual_machine.h"

CellBrain MakePatrolCell(const SimulationVirtualMachine& vm);