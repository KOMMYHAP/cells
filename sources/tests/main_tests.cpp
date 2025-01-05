#ifdef NDEBUG
#error You should run unit-tests in debug configuration to enable asserts in death tests!
#endif

#include "utils/test_throw_assert_listener.h"

int main(int argc, char** argv)
{
    testing::UnitTest::GetInstance()->listeners().Append(new ThrowTestListener);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}