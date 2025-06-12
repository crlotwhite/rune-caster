# Unit tests CMakeLists.txt

# Google Test를 찾습니다
find_package(GTest QUIET)

if(GTest_FOUND)
    # Rune 테스트
    add_executable(test_rune test_rune.cpp)
    target_link_libraries(test_rune PRIVATE
        rune_caster
        GTest::gtest
        GTest::gtest_main
    )

    # 테스트 등록
    include(GoogleTest)
    gtest_discover_tests(test_rune)

    message(STATUS "Unit tests enabled with Google Test")
else()
    message(STATUS "Google Test not found, skipping unit tests")
endif()