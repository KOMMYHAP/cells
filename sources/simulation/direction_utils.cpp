#include "direction_utils.h"

bool TryParseDirection(uint8_t data, Direction& direction)
{
    switch (data) {
    case static_cast<uint8_t>(Direction::Right):
        direction.type = Direction::Right;
        return true;
    case static_cast<uint8_t>(Direction::Left):
        direction.type = Direction::Left;
        return true;
    case static_cast<uint8_t>(Direction::Down):
        direction.type = Direction::Down;
        return true;
    case static_cast<uint8_t>(Direction::Up):
        direction.type = Direction::Up;
        return true;
    default:
        return false;
    }
}