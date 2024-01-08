#pragma once

#include "cell.h"
#include "field.h"

class Brain;

enum class UnitCommand : uint8_t {
    Nope,
    Move,
    Look,
    Jump,
    JumpIf
};

enum class UnitControlFlags : uint8_t {
    None = 0x00,
    ExecuteYetAnotherOne = 0x01,
    CommandOutOfRange = 0x01,
    OutOfField = 0x02
};
enum class Direction : uint8_t {
    Left,
    Right,
    Up,
    Down,
};

#pragma pack(push, 1)
struct UnitCommandParam {
    uint8_t value;
};
struct UnitControlBlock {
    uint8_t nextCommand;
    UnitCommandParam result;
    UnitControlFlags flags;
};
#pragma pack(pop)

struct UnitData {
    UnitCommand action;
    Direction moveDirection;
};

class UnitProcessor {
public:
    UnitProcessor(Brain& brain, Field& field);

    static Cell MakeDefaultUnit();

    void Process();

private:
    void ExecuteCommand();

    Brain& _brain;
    Field& _field;
};