#pragma once
#include "components/move_direction.h"

using Direction = MoveDirection;

bool TryParse(uint8_t data, Direction& direction);