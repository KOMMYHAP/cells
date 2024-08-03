// #include "preset_cell_factory.h"
//
// ICellFactory::Result PresetCellFactory::Make()
// {
//     Result result;
//     if (_brainIndex >= _brains.size()) {
//         result.status = Status::TryLater;
//         return result;
//     }
//
//     result.brain = _brains[_brainIndex];
//     result.status = Status::Success;
//     _brainIndex += 1;
//     return result;
// }
//
// void PresetCellFactory::SetPreset(std::vector<CellBrain> brains)
// {
//     _brainIndex = 0;
//     _brains = std::move(brains);
// }
