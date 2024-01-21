#pragma once

using CellPosition = sf::Vector2<int16_t>;

inline const static CellPosition InvalidCellPosition { std::numeric_limits<int16_t>::max(), std::numeric_limits<int16_t>::max() };