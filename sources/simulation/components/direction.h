#pragma once

enum class Direction : uint8_t {
    Left,
    Right,
    Up,
    Down,
};

bool TryParseDirection(uint8_t data, Direction& direction);
