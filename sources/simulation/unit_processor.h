#pragma once

#include "cell.h"
#include "field.h"

class Brain;
class BrainData;

enum class SystemCommand : uint8_t {
    Nope,
    Jump,
    JumpIf,
    Reserved1,
    Reserved2,
    Reserved3,
    Reserved4,
    Reserved5,
    Reserved6,
    Reserved7,
};
constexpr uint8_t SystemCommandCount = 10;

enum class UnitCommand : uint8_t {
    Move = SystemCommandCount,
    Look
};
constexpr uint8_t UnitCommandCount = 2;

enum class UnitControlFlags : uint8_t {
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

#pragma pack(push, 1)
struct UnitCommandParam {
    uint8_t value;
};
struct UnitControlBlock {
    uint8_t nextCommand;
    uint8_t r1;
    uint8_t r2;
    uint8_t r3;
    UnitControlFlags flags;
};
#pragma pack(pop)

class UnitProcessor {
public:
    UnitProcessor(Brain& brain, Field& field);

    static Cell MakeDefaultUnit();

    void Process();

private:
    void ProcessCommand();
    void ProcessSystemCommand(UnitControlBlock& controlBlock, BrainData brainData, SystemCommand command);
    void ProcessUnitCommand(UnitControlBlock& controlBlock, BrainData brainData, UnitCommand command);

    Brain& _brain;
    Field& _field;
};