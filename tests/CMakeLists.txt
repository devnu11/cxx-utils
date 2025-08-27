if(BUILD_TESTING)
    if(PROJECT_IS_TOP_LEVEL)
        # Add GoogleTest using FetchContent
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.15.2
        )

        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        find_package(Threads REQUIRED)
        FetchContent_MakeAvailable(googletest)

        include(GoogleTest)
    endif()


    add_executable(         TestCxxUtils                test_pnext_range.cpp)
    target_compile_features(TestCxxUtils    PRIVATE     cxx_std_23)
    target_link_libraries(  TestCxxUtils    PRIVATE     CxxUtils
                                                        GTest::gtest_main
                                                        GTest::gtest
    )
    gtest_discover_tests(   TestCxxUtils)
endif()