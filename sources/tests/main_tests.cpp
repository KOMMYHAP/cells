#ifdef NDEBUG
#error You should run unit-tests in debug configuration to enable asserts in death tests!
#endif

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}