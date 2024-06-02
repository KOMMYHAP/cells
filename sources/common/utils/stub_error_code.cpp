#include "stub_error_code.h"

#include "asserts/assert.h"

namespace common {
std::error_code MakeStubErrorCode()
{
    return {};
}

}