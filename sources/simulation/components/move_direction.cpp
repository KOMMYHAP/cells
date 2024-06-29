#include "move_direction.h"

bool TryParseMoveDirection(uint8_t data, MoveDirection& direction)
{
    switch (data) {
    case static_cast<uint8_t>(MoveDirection::Right):
        direction = MoveDirection::Right;
        return true;
    case static_cast<uint8_t>(MoveDirection::Left):
        direction = MoveDirection::Left;
        return true;
    case static_cast<uint8_t>(MoveDirection::Down):
        direction = MoveDirection::Down;
        return true;
    case static_cast<uint8_t>(MoveDirection::Up):
        direction = MoveDirection::Up;
        return true;
    default:
        return false;
    }
}