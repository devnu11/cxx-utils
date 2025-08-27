if(BUILD_TESTING)
    add_executable(         TestUtilities               test_pnext_range.cpp)
    target_compile_features(TestUtilities   PRIVATE     cxx_std_20)
    target_link_libraries(  TestUtilities   PRIVATE     Utilities
                                                        GTest::gtest_main
                                                        GTest::gtest
    )

    # Use gtest_discover_tests to register individual test cases with CTest
    include(GoogleTest)
    gtest_discover_tests(TestUtilities)
endif()