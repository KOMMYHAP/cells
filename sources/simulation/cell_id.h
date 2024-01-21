#pragma once

enum class CellId : uint32_t {
    Invalid = std::numeric_limits<uint32_t>::max()
};

constexpr auto CellIdToInt(CellId id) { return static_cast<std::underlying_type_t<CellId>>(id); }