#include "cell_position.h"

static_assert(std::is_signed_v<decltype(CellPosition::x)> && std::is_signed_v<decltype(CellPosition::x)>, "For vector math operations signed type is required");