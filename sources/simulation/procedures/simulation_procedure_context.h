#pragma once

#include "components/cell_id.h"

struct SimulationProcedureContext {
    const CellId* id; ///< Should be passed by pointer to storage of CellId in entt::registry for better data location. It's always not null.
};