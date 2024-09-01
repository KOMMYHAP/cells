#include "sdl_utils.h"

[[noreturn]] void PanicOnSdlError(const std::string_view functionName)
{
    std::cerr << std::format("%s failed: \"%s\"", functionName, SDL_GetError()) << std::endl;
    ASSERT_FAIL("SDL Error");
    std::abort();
}