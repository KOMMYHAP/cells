#pragma once
#include "components/cell_age.h"
#include "components/cell_brain.h"
#include "components/cell_health.h"
#include "components/cell_id.h"
#include "components/cell_position.h"
#include "components/cell_type.h"

struct SpawnProperties {
    CellPosition position { InvalidCellPosition };
    CellBrain brain {};
    CellHealth health { CellHealth ::Zero };
    CellType type { CellType::Dummy };
    CellAge age { CellAge::Zero };
};