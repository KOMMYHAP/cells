#include "sdl_panic.h"

[[noreturn]] void PanicOnSdlError(const std::string_view functionName)
{
    std::cerr << std::format("{} failed: \"{}\"", functionName, SDL_GetError()) << std::endl;
    ASSERT_FAIL("SDL Error");
    std::abort();
}