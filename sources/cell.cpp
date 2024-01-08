#include "cell.h"

std::string_view TypeToStr(Type type)
{
    switch (type) {
    case Type::Unit:
        return "Unit";
    case Type::Food:
        return "Food";
    case Type::Wall:
        return "Wall";
    case Type::Dummy:
        return "Dummy";
    default:
        assert(false);
        return "unknown";
    }
}
