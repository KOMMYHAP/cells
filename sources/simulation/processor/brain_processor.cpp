#include "brain_processor.h"
#include "brain/brain.h"

static bool TryApplyDirection(sf::Vector2<uint16_t>& position, const sf::Vector2u& limits, Direction direction)
{
    switch (direction) {
    case Direction::Left:
        if (position.x >= 1) {
            position.x -= 1;
            return true;
        }
        break;
    case Direction::Right:
        if (position.x + 1 < limits.x) {
            position.x += 1;
            return true;
        }
        break;
    case Direction::Up:
        if (position.y >= 1) {
            position.y -= 1;
            return true;
        }
        break;
    case Direction::Down:
        if (position.y + 1 < limits.y) {
            position.y += 1;
            return true;
        }
        break;
    }

    return false;
}

template <class T>
static bool HasFlag(T value, T flag)
{
    const auto v = static_cast<std::underlying_type_t<T>>(value);
    const auto f = static_cast<std::underlying_type_t<T>>(flag);
    return (v & f) == f;
}

template <class T>
static void SetFlag(T& value, T flag, bool enabled = true)
{
    const auto v = static_cast<std::underlying_type_t<T>>(value);
    const auto f = static_cast<std::underlying_type_t<T>>(flag);
    if (enabled) {
        value = static_cast<T>(v | f);
    } else {
        value = static_cast<T>(v & (!f));
    }
}

BrainProcessor::BrainProcessor(CellId cellId, Brain& brain, Field& field)
    : _brain(brain)
    , _field(field)
    , _cellId(cellId)
{
}

void BrainProcessor::Process()
{
    Memory brainData = _brain.AccessMemory();
    if (!brainData.HasBytes<BrainControlBlock>()) {
        assert(false);
        return;
    }
    BrainControlBlock& controlBlock = brainData.Get<BrainControlBlock>();
    if (HasFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange)) {
        return;
    }

    ProcessCommand();

    constexpr int extraCommandsPerTickLimit = 1;
    for (int i = 0; i < extraCommandsPerTickLimit; ++i) {
        if (HasFlag(controlBlock.flags, CommandControlFlags::ExecuteYetAnotherOne)) {
            ProcessCommand();
        }
    }
}

Cell BrainProcessor::MakeDefaultUnit()
{
    Cell defaultUnitCell;
    Brain brain { defaultUnitCell };
    brain.AccessInfo().type = CellType::Unit;

    Memory brainData = brain.AccessMemory();
    if (!brainData.HasBytes(sizeof(BrainControlBlock) + sizeof(CommandParam) * 2)) {
        assert(false);
        return Cell();
    }

    BrainControlBlock& controlBlock = brainData.Get<BrainControlBlock>();
    controlBlock = BrainControlBlock {};
    controlBlock.commandOffset = 0;
    controlBlock.flags = CommandControlFlags::None;

    brainData.Write(SystemCommand::Jump, CommandParam{0});

    return defaultUnitCell;
}

void BrainProcessor::ProcessCommand()
{
    Memory brainData = _brain.AccessMemory();
    BrainControlBlock& controlBlock = brainData.Get<BrainControlBlock>();

    SetFlag(controlBlock.flags, CommandControlFlags::ExecuteYetAnotherOne, false);
    SetFlag(controlBlock.flags, CommandControlFlags::OutOfField, false);

    if (!brainData.HasBytes(controlBlock.nextCommand)) {
        SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
        return;
    }
    brainData.Move(controlBlock.nextCommand);

    if (!brainData.HasBytes<UnitCommand>()) {
        SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
        return;
    }
    const auto param = brainData.Get<CommandParam>();

    if (param.value < SystemCommandCount) {
        ProcessSystemCommand(controlBlock, brainData, static_cast<SystemCommand>(param.value));
    } else if (param.value >= SystemCommandCount && param.value < SystemCommandCount + UnitCommandCount) {
        ProcessUnitCommand(controlBlock, brainData, static_cast<UnitCommand>(param.value));
    }
}

void BrainProcessor::ProcessSystemCommand(BrainControlBlock& controlBlock, Memory brainData, SystemCommand command)
{
    switch (command) {
    case SystemCommand::Nope:
        break;
    case SystemCommand::JumpIf: {
        if (!brainData.HasBytes<CommandParam>()) {
            SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
            break;
        }
        const auto condition = brainData.Get<CommandParam>();

        if (!brainData.HasBytes<CommandParam>()) {
            SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
            break;
        }
        const auto destination = brainData.Get<CommandParam>();

        if (controlBlock.r1 == condition.value) {
            controlBlock.nextCommand = destination.value;
        } else {
            controlBlock.nextCommand += 3;
        }
        SetFlag(controlBlock.flags, CommandControlFlags::ExecuteYetAnotherOne);
    } break;
    case SystemCommand::Jump: {
        if (!brainData.HasBytes<CommandParam>()) {
            SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
            break;
        }
        const auto destination = brainData.Get<CommandParam>();
        controlBlock.nextCommand = destination.value;
        SetFlag(controlBlock.flags, CommandControlFlags::ExecuteYetAnotherOne);
    } break;
    default: {
        SetFlag(controlBlock.flags, CommandControlFlags::InvalidCommand);
        break;
    }
    }
}

void BrainProcessor::ProcessUnitCommand(BrainControlBlock& controlBlock, Memory brainData, UnitCommand command)
{
    switch (command) {
    case UnitCommand::Move: {
        if (!brainData.HasBytes<Direction>()) {
            SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
            break;
        }
        const auto direction = brainData.Get<Direction>();
        auto nextPosition = _brain.AccessInfo().position;
        const bool applied = TryApplyDirection(nextPosition, _field.GetPositionLimits(), direction);
        if (applied) {
            _field.Move(_cellId, nextPosition);
        } else {
            SetFlag(controlBlock.flags, CommandControlFlags::OutOfField);
        }

        controlBlock.nextCommand += 2;
    } break;
    case UnitCommand::Look: {
        if (!brainData.HasBytes<Direction>()) {
            SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
            break;
        }
        const auto direction = brainData.Get<Direction>();
        auto position = _brain.AccessInfo().position;
        const bool applied = TryApplyDirection(position, _field.GetPositionLimits(), direction);
        CellType type = CellType::Dummy;
        if (applied) {
            std::vector<CellId> cells = _field.Find({ position.x, position.y });
            if (!cells.empty()) {
                const Cell& cell = _field.Get(cells.back());
                type = ConstBrain(cell).GetInfo().type;
            }
        } else {
            SetFlag(controlBlock.flags, CommandControlFlags::OutOfField);
        }

        controlBlock.r1 = static_cast<std::underlying_type_t<CellType>>(type);
        controlBlock.nextCommand += 2;
    } break;
    }
}
