#include "test_throw_assert_listener.h"

void ThrowTestListener::OnTestPartResult(const testing::TestPartResult& result)
{
    if (result.type() == testing::TestPartResult::kFatalFailure) {
        throw testing::AssertionException(result);
    }
}