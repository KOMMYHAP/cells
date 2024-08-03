#pragma once
#include "components/cell_brain.h"
#include "simulation/simulation_virtual_machine.h"

CellBrain MakePatrolCell(const SimulationVirtualMachine& vm);