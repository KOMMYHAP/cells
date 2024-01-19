#pragma once

#include "cell.h"

constexpr uint8_t SystemCommandCount = 10;

enum class UnitCommand : uint8_t {
    Move = SystemCommandCount,
    Look
};
constexpr uint8_t UnitCommandCount = 2;

enum class CommandControlFlags : uint8_t {
    None = 0x00,
    ExecuteYetAnotherOne = 1 << 1,
    CommandOutOfRange = 1 << 2,
    OutOfField = 1 << 3,
    InvalidCommand = 1 << 4
};

enum class Direction : uint8_t {
    Left,
    Right,
    Up,
    Down,
};

enum class CellType : uint8_t {
    Unit,
    Food,
    Wall,
    Dummy
};

#pragma pack(push, 1)
struct CellInfo {
    CellType type;
    CellPosition position;
};

struct CommandParam {
    uint8_t value;
};

struct BrainControlBlock {
    uint8_t commandOffset;
    uint8_t nextCommand;
    uint8_t r1;
    CommandControlFlags flags;
};
#pragma pack(pop)