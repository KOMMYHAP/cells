#include "sdl_panic.h"

[[noreturn]] void PanicOnSdlError(const std::string_view functionName)
{
    std::println("{} failed: \"{}\"", functionName, SDL_GetError());
    ASSERT_FAIL("SDL Error");
    std::abort();
}
