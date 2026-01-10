#pragma once
#include "components/generated/auto_cell_position.h"

class GameController {
public:
    float GetFramesPerSecond() const;
    CellPosition GetGravityCenter() const;

private:
};