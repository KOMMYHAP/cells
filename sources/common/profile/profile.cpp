#include "profile.h"

#include <windows.h>

#include "WinPixEventRuntime/pix3.h"

namespace common::details {

void ProfileBeginEvent(std::string_view name, ProfileCategory category)
{
    PIXBeginEvent(PIX_COLOR_INDEX(static_cast<uint8_t>(category)), "%s", name.data());
}

void ProfileEndEvent()
{
    PIXEndEvent();
}

}

namespace common {

ProfileCategory MakeProfileCategory()
{
    static ProfileCategory nextCategory { 0 };
    if (nextCategory == ProfileCategory::Invalid) {
        return ProfileCategory::Invalid;
    }

    const auto value = static_cast<std::underlying_type_t<ProfileCategory>>(nextCategory);
    nextCategory = static_cast<ProfileCategory>(value + 1);
    return static_cast<ProfileCategory>(value);
}

}