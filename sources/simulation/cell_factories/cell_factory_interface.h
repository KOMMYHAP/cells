#pragma once

#include "components/cell_brain.h"
#include "components/cell_position.h"

class ICellFactory {
public:
    virtual ~ICellFactory() = default;

    struct Parent {
        CellId id;
        gsl::not_null<const CellBrain*> brain;
        CellPosition position;
    };
    struct Result {
        CellBrain brain;
        bool success { false };
    };
    virtual Result Make(const Parent& parent) = 0;
};