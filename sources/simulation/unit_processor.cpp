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

UnitProcessor::UnitProcessor(Brain& brain, Field& field)
    : _brain(brain)
    , _field(field)
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

    ExecuteCommand();

    constexpr int extraCommandsPerTickLimit = 1;
    for (int i = 0; i < extraCommandsPerTickLimit; ++i) {
        if (HasFlag(controlBlock.flags, UnitControlFlags::ExecuteYetAnotherOne)) {
            ExecuteCommand();
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
    controlBlock.flags = UnitControlFlags::None;
    controlBlock.nextCommand = 0;
    controlBlock.result = UnitCommandParam { 0 };

    auto& jump = brainData.Pop<UnitCommandParam>();
    jump.value = static_cast<std::underlying_type_t<UnitCommand>>(UnitCommand::Jump);

    auto& jumpDestination = brainData.Pop<UnitCommandParam>();
    jump.value = 0; // jump to command 'jump(0)'

    return defaultUnitCell;
}

void UnitProcessor::ExecuteCommand()
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
    const UnitCommand command = brainData.Pop<UnitCommand>();

    switch (command) {
    case UnitCommand::Nope:
        break;
    case UnitCommand::Move: {
        if (!brainData.HasBytes<Direction>()) {
            SetFlag(controlBlock.flags, UnitControlFlags::CommandOutOfRange);
            break;
        }
        const auto direction = brainData.Pop<Direction>();
        auto& modifiedPosition = _brain.AccessInfo().position;
        const bool applied = TryApplyDirection(modifiedPosition, _field.GetPositionLimits(), direction);
        if (!applied) {
            SetFlag(controlBlock.flags, UnitControlFlags::OutOfField);
            break;
        }

        // todo: propage CellId
        _field.NotifyMoved(CellId { 0 });
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
        if (!applied) {
            SetFlag(controlBlock.flags, UnitControlFlags::OutOfField);
        }
        std::vector<CellId> cells = _field.Find({ position.x, position.y });
        CellType type = CellType::Dummy;
        if (!cells.empty()) {
            const Cell& cell = _field.Get(cells.back());
            type = cell.GetBrain().GetType();
        }
        controlBlock.result.value = static_cast<std::underlying_type_t<CellType>>(type);
        controlBlock.nextCommand += 2;
    } break;
    case UnitCommand::JumpIf: {
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

        if (controlBlock.result.value == condition.value) {
            controlBlock.nextCommand = destination.value;
        } else {
            controlBlock.nextCommand += 3;
        }
        SetFlag(controlBlock.flags, UnitControlFlags::ExecuteYetAnotherOne);
    } break;
    case UnitCommand::Jump: {
        if (!brainData.HasBytes<UnitCommandParam>()) {
            SetFlag(controlBlock.flags, UnitControlFlags::CommandOutOfRange);
            break;
        }
        const auto destination = brainData.Pop<UnitCommandParam>();
        controlBlock.nextCommand = destination.value;
        SetFlag(controlBlock.flags, UnitControlFlags::ExecuteYetAnotherOne);
    } break;
    }
}
