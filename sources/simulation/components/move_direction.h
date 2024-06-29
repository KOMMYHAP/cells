#pragma once

enum class MoveDirection : uint8_t {
    Left,
    Right,
    Up,
    Down,
};

bool TryParseMoveDirection(uint8_t data, MoveDirection& direction);
