#include "breakpoint.h"

int main(int argc, char** argv)
{
    common::EnableBreakpointOnAssert(true);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}