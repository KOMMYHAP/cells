#include "unit_processor.h"
#include "brain.h"

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

UnitProcessor::UnitProcessor(CellId cellId, Brain& brain, Field& field)
    : _brain(brain)
    , _field(field)
    , _cellId(cellId)
{
}
void UnitProcessor::Process()
{
    BrainData brainData = _brain.AccessData();
    if (!brainData.HasBytes<UnitControlBlock>()) {
        assert(false);
        return;
    }
    UnitControlBlock& controlBlock = brainData.Pop<UnitControlBlock>();
    if (HasFlag(controlBlock.flags, UnitControlFlags::CommandOutOfRange)) {
        return;
    }

    ProcessCommand();

    constexpr int extraCommandsPerTickLimit = 1;
    for (int i = 0; i < extraCommandsPerTickLimit; ++i) {
        if (HasFlag(controlBlock.flags, UnitControlFlags::ExecuteYetAnotherOne)) {
            ProcessCommand();
        }
    }
}

Cell UnitProcessor::MakeDefaultUnit()
{
    Cell defaultUnitCell;
    Brain brain = defaultUnitCell.GetBrain();
    brain.AccessInfo().type = CellType::Unit;

    BrainData brainData = brain.AccessData();
    if (!brainData.HasBytes(sizeof(UnitControlBlock) + sizeof(UnitCommandParam) * 2)) {
        assert(false);
        return Cell();
    }

    UnitControlBlock& controlBlock = brainData.Pop<UnitControlBlock>();
    controlBlock = UnitControlBlock {};
    controlBlock.flags = UnitControlFlags::None;

    auto& jump = brainData.Pop<UnitCommandParam>();
    jump.value = static_cast<std::underlying_type_t<SystemCommand>>(SystemCommand::Jump);

    auto& jumpDestination = brainData.Pop<UnitCommandParam>();
    jumpDestination.value = 0; // jump to command 'jump(0)'

    return defaultUnitCell;
}

void UnitProcessor::ProcessCommand()
{
    BrainData brainData = _brain.AccessData();
    UnitControlBlock& controlBlock = brainData.Pop<UnitControlBlock>();

    SetFlag(controlBlock.flags, UnitControlFlags::ExecuteYetAnotherOne, false);
    SetFlag(controlBlock.flags, UnitControlFlags::OutOfField, false);

    if (!brainData.HasBytes(controlBlock.nextCommand)) {
        SetFlag(controlBlock.flags, UnitControlFlags::CommandOutOfRange);
        return;
    }
    brainData.Move(controlBlock.nextCommand);

    if (!brainData.HasBytes<UnitCommand>()) {
        SetFlag(controlBlock.flags, UnitControlFlags::CommandOutOfRange);
        return;
    }
    const auto param = brainData.Pop<UnitCommandParam>();

    if (param.value < SystemCommandCount) {
        ProcessSystemCommand(controlBlock, brainData, static_cast<SystemCommand>(param.value));
    } else if (param.value >= SystemCommandCount && param.value < SystemCommandCount + UnitCommandCount) {
        ProcessUnitCommand(controlBlock, brainData, static_cast<UnitCommand>(param.value));
    }
}

void UnitProcessor::ProcessSystemCommand(UnitControlBlock& controlBlock, BrainData brainData, SystemCommand command)
{
    switch (command) {
    case SystemCommand::Nope:
        break;
    case SystemCommand::JumpIf: {
        if (!brainData.HasBytes<UnitCommandParam>()) {
            SetFlag(controlBlock.flags, UnitControlFlags::CommandOutOfRange);
            break;
        }
        const auto condition = brainData.Pop<UnitCommandParam>();

        if (!brainData.HasBytes<UnitCommandParam>()) {
            SetFlag(controlBlock.flags, UnitControlFlags::CommandOutOfRange);
            break;
        }
        const auto destination = brainData.Pop<UnitCommandParam>();

        if (controlBlock.r1 == condition.value) {
            controlBlock.nextCommand = destination.value;
        } else {
            controlBlock.nextCommand += 3;
        }
        SetFlag(controlBlock.flags, UnitControlFlags::ExecuteYetAnotherOne);
    } break;
    case SystemCommand::Jump: {
        if (!brainData.HasBytes<UnitCommandParam>()) {
            SetFlag(controlBlock.flags, UnitControlFlags::CommandOutOfRange);
            break;
        }
        const auto destination = brainData.Pop<UnitCommandParam>();
        controlBlock.nextCommand = destination.value;
        SetFlag(controlBlock.flags, UnitControlFlags::ExecuteYetAnotherOne);
    } break;
    default: {
        SetFlag(controlBlock.flags, UnitControlFlags::InvalidCommand);
        break;
    }
    }
}

void UnitProcessor::ProcessUnitCommand(UnitControlBlock& controlBlock, BrainData brainData, UnitCommand command)
{
    switch (command) {
    case UnitCommand::Move: {
        if (!brainData.HasBytes<Direction>()) {
            SetFlag(controlBlock.flags, UnitControlFlags::CommandOutOfRange);
            break;
        }
        const auto direction = brainData.Pop<Direction>();
        auto nextPosition = _brain.AccessInfo().position;
        const bool applied = TryApplyDirection(nextPosition, _field.GetPositionLimits(), direction);
        if (applied) {
            _field.Move(_cellId, nextPosition);
        } else {
            SetFlag(controlBlock.flags, UnitControlFlags::OutOfField);
        }

        controlBlock.nextCommand += 2;
    } break;
    case UnitCommand::Look: {
        if (!brainData.HasBytes<Direction>()) {
            SetFlag(controlBlock.flags, UnitControlFlags::CommandOutOfRange);
            break;
        }
        const auto direction = brainData.Pop<Direction>();
        auto position = _brain.AccessInfo().position;
        const bool applied = TryApplyDirection(position, _field.GetPositionLimits(), direction);
        CellType type = CellType::Dummy;
        if (applied) {
            std::vector<CellId> cells = _field.Find({ position.x, position.y });
            if (!cells.empty()) {
                const Cell& cell = _field.Get(cells.back());
                type = cell.GetBrain().GetType();
            }
        } else {
            SetFlag(controlBlock.flags, UnitControlFlags::OutOfField);
        }

        controlBlock.r1 = static_cast<std::underlying_type_t<CellType>>(type);
        controlBlock.nextCommand += 2;
    } break;
    }
}
