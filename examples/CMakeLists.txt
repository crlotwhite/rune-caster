# Examples CMakeLists.txt

# Basic usage example
add_executable(basic_usage basic_usage.cpp)
target_link_libraries(basic_usage PRIVATE rune_caster)

# Sequence usage example
add_executable(sequence_usage sequence_usage.cpp)
target_link_libraries(sequence_usage PRIVATE rune_caster)

# Unicode debug test
add_executable(unicode_test unicode_test.cpp)
target_link_libraries(unicode_test PRIVATE rune_caster)

# Spell usage example
add_executable(spell_usage spell_usage.cpp)
target_link_libraries(spell_usage PRIVATE rune_caster)

# MSVC UTF-8 설정
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(basic_usage PRIVATE /utf-8 /wd4819)
    target_compile_options(sequence_usage PRIVATE /utf-8 /wd4819)
    target_compile_options(unicode_test PRIVATE /utf-8 /wd4819)
    target_compile_options(spell_usage PRIVATE /utf-8 /wd4819)
endif()

# Set output directory
set_target_properties(basic_usage sequence_usage unicode_test spell_usage PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)