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

# CaseConverter usage example
add_executable(case_converter_usage case_converter_usage.cpp)
target_link_libraries(case_converter_usage PRIVATE rune_caster)

# Caster pipeline demo
add_executable(caster_pipeline_demo caster_pipeline_demo.cpp)
target_link_libraries(caster_pipeline_demo PRIVATE rune_caster)

# Advanced GCC 15 concepts demo
add_executable(gcc15_concepts_demo gcc15_concepts_demo.cpp)
target_link_libraries(gcc15_concepts_demo PRIVATE rune_caster)
target_compile_features(gcc15_concepts_demo PRIVATE cxx_std_20)

# Enhanced spell factory demo
add_executable(spell_demo spell_demo.cpp)
target_link_libraries(spell_demo PRIVATE rune_caster)
target_compile_features(spell_demo PRIVATE cxx_std_20)

# MSVC UTF-8 설정
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(basic_usage PRIVATE /utf-8 /wd4819)
    target_compile_options(sequence_usage PRIVATE /utf-8 /wd4819)
    target_compile_options(unicode_test PRIVATE /utf-8 /wd4819)
    target_compile_options(spell_usage PRIVATE /utf-8 /wd4819)
    target_compile_options(case_converter_usage PRIVATE /utf-8 /wd4819)
    target_compile_options(caster_pipeline_demo PRIVATE /utf-8 /wd4819)
    target_compile_options(gcc15_concepts_demo PRIVATE /utf-8 /wd4819)
    target_compile_options(spell_demo PRIVATE /utf-8 /wd4819)
endif()

# Set output directory
set_target_properties(basic_usage sequence_usage unicode_test spell_usage case_converter_usage caster_pipeline_demo gcc15_concepts_demo spell_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)