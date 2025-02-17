#include "direction_utils.h"

bool TryParseDirection(uint8_t data, Direction& direction)
{
    switch (data) {
    case static_cast<uint8_t>(Direction::Right):
        direction = Direction::Right;
        return true;
    case static_cast<uint8_t>(Direction::Left):
        direction = Direction::Left;
        return true;
    case static_cast<uint8_t>(Direction::Down):
        direction = Direction::Down;
        return true;
    case static_cast<uint8_t>(Direction::Up):
        direction = Direction::Up;
        return true;
    default:
        return false;
    }
}