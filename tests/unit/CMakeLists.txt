# Unit tests CMakeLists.txt

# Google Test를 FetchContent로 자동 다운로드
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_SHALLOW    TRUE
)

# Windows에서 gtest_force_shared_crt 설정
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

message(STATUS "Google Test fetched successfully")

# Rune 테스트
add_executable(test_rune test_rune.cpp)
target_link_libraries(test_rune PRIVATE
    rune_caster
    gtest
    gtest_main
)

# Spell 테스트
add_executable(test_spell test_spell.cpp)
target_link_libraries(test_spell PRIVATE
    rune_caster
    gtest
    gtest_main
)

# 테스트 등록
include(GoogleTest)
gtest_discover_tests(test_rune)
gtest_discover_tests(test_spell)