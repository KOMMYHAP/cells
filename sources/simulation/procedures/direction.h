#pragma once

enum class Direction : uint8_t {
    Left,
    Right,
    Up,
    Down,
};

bool TryParse(uint8_t data, Direction& direction);