#pragma once
#include <cstdint>
#include <limits>

namespace Common {

enum class Condition : uint16_t {
    Invalid = std::numeric_limits<uint16_t>::max()
};

}