// #pragma once
//
// #include "components/cell_brain.h"
//
// class ICellFactory {
// public:
//     virtual ~ICellFactory() = default;
//
//     enum class Status : uint8_t {
//         Success,
//         FailedToCreate,
//         TryLater
//     };
//     struct Result {
//         Status status { Status::TryLater };
//         CellBrain brain;
//     };
//     virtual Result Make() = 0;
// };