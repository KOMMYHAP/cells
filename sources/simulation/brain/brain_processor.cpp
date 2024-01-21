//#include "brain_processor.h"
//#include "data.h"
//#include "flags.h"
//#include "processor/processor_control_block.h"
//
//static bool TryApplyDirection(CellPosition& position, const World& world, Direction direction)
//{
//    switch (direction) {
//    case Direction::Left:
//        if (position.x >= 1) {
//            position.x -= 1;
//            return true;
//        }
//        break;
//    case Direction::Right:
//        if (position.x + 1 < world.GetWidth()) {
//            position.x += 1;
//            return true;
//        }
//        break;
//    case Direction::Up:
//        if (position.y >= 1) {
//            position.y -= 1;
//            return true;
//        }
//        break;
//    case Direction::Down:
//        if (position.y + 1 < world.GetHeight()) {
//            position.y += 1;
//            return true;
//        }
//        break;
//    }
//
//    return false;
//}
//
//BrainProcessor::BrainProcessor(CellId cellId, Brain& data, World& world)
//    : _brain(data)
//    , _world(world)
//    , _id(cellId)
//{
//}
//
//void BrainProcessor::Process()
//{
//    Memory brainData = _brain.AccessMemory();
//    if (!brainData.HasBytes<BrainControlBlock>()) {
//        assert(false);
//        return;
//    }
//    BrainControlBlock& controlBlock = brainData.Get<BrainControlBlock>();
//    if (common::HasFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange)) {
//        return;
//    }
//
//    ProcessCommand();
//
//    constexpr int extraCommandsPerTickLimit = 1;
//    for (int i = 0; i < extraCommandsPerTickLimit; ++i) {
//        if (common::HasFlag(controlBlock.flags, CommandControlFlags::ExecuteYetAnotherOne)) {
//            ProcessCommand();
//        }
//    }
//}
//
//Brain BrainProcessor::MakeDefaultUnit()
//{
//    Brain defaultUnitCell;
//    Brain data { defaultUnitCell };
//
//    Memory memory = data.AccessMemory();
//    if (!memory.HasBytes(sizeof(BrainControlBlock) + sizeof(CommandParam) * 2)) {
//        assert(false);
//        return Brain();
//    }
//
//    BrainControlBlock& controlBlock = memory.Get<BrainControlBlock>();
//    controlBlock = BrainControlBlock {};
//    controlBlock.commandOffset = 0;
//    controlBlock.flags = static_cast<uint8_t>(CommandControlFlags::None);
//
//    memory.Write(ProcessorInstruction::Jump, CommandParam { 0 });
//
//    return defaultUnitCell;
//}
//
//void BrainProcessor::ProcessCommand()
//{
//    Memory memory = _brain.AccessMemory();
//    BrainControlBlock& controlBlock = memory.Get<BrainControlBlock>();
//
//    common::ResetFlag(controlBlock.flags, CommandControlFlags::ExecuteYetAnotherOne);
//    common::ResetFlag(controlBlock.flags, CommandControlFlags::OutOfField);
//
//    if (!memory.HasBytes(controlBlock.nextCommand)) {
//        common::SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
//        return;
//    }
//    memory.Move(controlBlock.nextCommand);
//
//    if (!memory.HasBytes<UnitCommand>()) {
//        common::SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
//        return;
//    }
//    const auto param = memory.Get<CommandParam>();
//
//    if (param.value < SystemCommandCount) {
//        ProcessSystemCommand(controlBlock, memory, static_cast<ProcessorInstruction>(param.value));
//    } else if (param.value >= SystemCommandCount && param.value < SystemCommandCount + UnitCommandCount) {
//        ProcessUnitCommand(controlBlock, memory, static_cast<UnitCommand>(param.value));
//    }
//}
//
//void BrainProcessor::ProcessSystemCommand(BrainControlBlock& controlBlock, Memory brainData, ProcessorInstruction command)
//{
//    switch (command) {
//    case ProcessorInstruction::Nope:
//        controlBlock.nextCommand += 1;
//        break;
//        //    case ProcessorInstruction::JumpIf: {
//        //        if (!brainData.HasBytes<CommandParam>()) {
//        //            SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
//        //            break;
//        //        }
//        //        const auto condition = brainData.Get<CommandParam>();
//        //
//        //        if (!brainData.HasBytes<CommandParam>()) {
//        //            SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
//        //            break;
//        //        }
//        //        const auto destination = brainData.Get<CommandParam>();
//        //
//        //        if (controlBlock.r1 == condition.value) {
//        //            controlBlock.nextCommand = destination.value;
//        //        } else {
//        //            controlBlock.nextCommand += 3;
//        //        }
//        //        SetFlag(controlBlock.flags, CommandControlFlags::ExecuteYetAnotherOne);
//        //    } break;
//    case ProcessorInstruction::Jump: {
//        if (!brainData.HasBytes<CommandParam>()) {
//            common::SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
//            break;
//        }
//        const auto destination = brainData.Get<CommandParam>();
//        controlBlock.nextCommand = destination.value;
//        common::SetFlag(controlBlock.flags, CommandControlFlags::ExecuteYetAnotherOne);
//    } break;
//    default: {
//        common::SetFlag(controlBlock.flags, CommandControlFlags::InvalidCommand);
//        break;
//    }
//    }
//}
//
//void BrainProcessor::ProcessUnitCommand(BrainControlBlock& controlBlock, Memory brainData, UnitCommand command)
//{
//    switch (command) {
//    case UnitCommand::Move: {
//        if (!brainData.HasBytes<Direction>()) {
//            common::SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
//            break;
//        }
//        const auto direction = brainData.Get<Direction>();
//        auto nextPosition = _world.positionSystem.Get(_id);
//        const bool applied = TryApplyDirection(nextPosition, _world, direction);
//        if (applied) {
//            const CellId id = _world.positionSystem.Find(nextPosition);
//            if (id == CellId::Invalid) {
//                _world.positionSystem.Move(_id, nextPosition);
//            }
//        } else {
//            common::SetFlag(controlBlock.flags, CommandControlFlags::OutOfField);
//        }
//
//        controlBlock.nextCommand += 2;
//    } break;
//    case UnitCommand::Look: {
//        if (!brainData.HasBytes<Direction>()) {
//            common::SetFlag(controlBlock.flags, CommandControlFlags::CommandOutOfRange);
//            break;
//        }
//        const auto direction = brainData.Get<Direction>();
//        auto position = _world.positionSystem.Get(_id);
//        const bool applied = TryApplyDirection(position, _world, direction);
//        CellType type = CellType::Dummy;
//        if (applied) {
//            const CellId id = _world.positionSystem.Find(position);
//            if (id != CellId::Invalid) {
//                type = _world.typeSystem.Get(id);
//            }
//        } else {
//            common::SetFlag(controlBlock.flags, CommandControlFlags::OutOfField);
//        }
//
//        controlBlock.r1 = static_cast<std::underlying_type_t<CellType>>(type);
//        controlBlock.nextCommand += 2;
//    } break;
//    }
//}
