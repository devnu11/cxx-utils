if(BUILD_TESTING)
    add_executable(         TestCxxUtils                test_pnext_range.cpp)
    target_compile_features(TestCxxUtils    PRIVATE     cxx_std_20)
    target_link_libraries(  TestCxxUtils    PRIVATE     CxxUtils
                                                        GTest::gtest_main
                                                        GTest::gtest
    )

    # Use gtest_discover_tests to register individual test cases with CTest
    include(GoogleTest)
    gtest_discover_tests(TestCxxUtils)
endif()