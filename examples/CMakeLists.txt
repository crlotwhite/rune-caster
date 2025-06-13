# ================================================================
# Rune Caster 예제 코드 빌드 설정
# ================================================================
#
# 이 파일은 Rune Caster 라이브러리의 핵심 예제들을 빌드하기 위한
# CMake 설정을 제공합니다.
#
# 예제 구성:
# - 기초 예제: Rune 및 RuneSequence 기본 사용법
# - 스펠 예제: 텍스트 처리 및 변환 기능
# - 파이프라인 예제: 고급 체이닝 및 워크플로우
# ================================================================

# 공통 컴파일 옵션 설정
set(EXAMPLE_COMPILE_OPTIONS)
set(EXAMPLE_LINK_LIBRARIES rune_caster)

# MSVC 특별 설정 (UTF-8 지원)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND EXAMPLE_COMPILE_OPTIONS /utf-8 /wd4819)
    message(STATUS "MSVC UTF-8 support enabled for examples")
endif()

# ================================================================
# 핵심 예제들 (Core Examples)
# ================================================================

# 기본 Rune 사용법
add_executable(basic_usage basic_usage.cpp)
target_compile_options(basic_usage PRIVATE ${EXAMPLE_COMPILE_OPTIONS})
target_link_libraries(basic_usage PRIVATE ${EXAMPLE_LINK_LIBRARIES})
set_target_properties(basic_usage PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# RuneSequence 사용법
add_executable(sequence_usage sequence_usage.cpp)
target_compile_options(sequence_usage PRIVATE ${EXAMPLE_COMPILE_OPTIONS})
target_link_libraries(sequence_usage PRIVATE ${EXAMPLE_LINK_LIBRARIES})
set_target_properties(sequence_usage PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# 스펠 시스템 데모
add_executable(spell_demo spell_demo.cpp)
target_compile_options(spell_demo PRIVATE ${EXAMPLE_COMPILE_OPTIONS})
target_link_libraries(spell_demo PRIVATE ${EXAMPLE_LINK_LIBRARIES})
set_target_properties(spell_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# 케이스 변환 사용법
add_executable(case_converter_usage case_converter_usage.cpp)
target_compile_options(case_converter_usage PRIVATE ${EXAMPLE_COMPILE_OPTIONS})
target_link_libraries(case_converter_usage PRIVATE ${EXAMPLE_LINK_LIBRARIES})
set_target_properties(case_converter_usage PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# 캐스터 파이프라인 데모
add_executable(caster_pipeline_demo caster_pipeline_demo.cpp)
target_compile_options(caster_pipeline_demo PRIVATE ${EXAMPLE_COMPILE_OPTIONS})
target_link_libraries(caster_pipeline_demo PRIVATE ${EXAMPLE_LINK_LIBRARIES})
set_target_properties(caster_pipeline_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# ================================================================
# 통합 타겟 설정
# ================================================================

# 모든 예제를 빌드하는 통합 타겟
add_custom_target(examples_all
    DEPENDS
        basic_usage
        sequence_usage
        spell_demo
        case_converter_usage
        caster_pipeline_demo
    COMMENT "Building all Rune Caster examples"
)

# 빌드 후 메시지
add_custom_command(TARGET examples_all POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "🎉 All Rune Caster examples built successfully!"
    COMMAND ${CMAKE_COMMAND} -E echo "📂 Examples located at: ${CMAKE_BINARY_DIR}/examples"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "📋 Available examples:"
    COMMAND ${CMAKE_COMMAND} -E echo "  - basic_usage: Rune class fundamentals"
    COMMAND ${CMAKE_COMMAND} -E echo "  - sequence_usage: RuneSequence container operations"
    COMMAND ${CMAKE_COMMAND} -E echo "  - spell_demo: Comprehensive spell system showcase"
    COMMAND ${CMAKE_COMMAND} -E echo "  - case_converter_usage: Text case transformation"
    COMMAND ${CMAKE_COMMAND} -E echo "  - caster_pipeline_demo: Advanced pipeline workflows"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "🚀 Run examples from the build directory:"
    COMMAND ${CMAKE_COMMAND} -E echo "  ./examples/basic_usage"
    COMMAND ${CMAKE_COMMAND} -E echo ""
)

# ================================================================
# 디버그 정보
# ================================================================

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Examples will be built with debug information")
    target_compile_definitions(basic_usage PRIVATE RUNE_CASTER_DEBUG=1)
    target_compile_definitions(sequence_usage PRIVATE RUNE_CASTER_DEBUG=1)
    target_compile_definitions(spell_demo PRIVATE RUNE_CASTER_DEBUG=1)
    target_compile_definitions(case_converter_usage PRIVATE RUNE_CASTER_DEBUG=1)
    target_compile_definitions(caster_pipeline_demo PRIVATE RUNE_CASTER_DEBUG=1)
endif()

# ================================================================
# 설치 설정 (선택적)
# ================================================================

if(RUNE_CASTER_INSTALL_EXAMPLES)
    install(TARGETS
        basic_usage
        sequence_usage
        spell_demo
        case_converter_usage
        caster_pipeline_demo
        RUNTIME DESTINATION examples
        COMPONENT examples
    )

    install(FILES README.md
        DESTINATION examples
        COMPONENT examples
    )

    message(STATUS "Examples will be installed to: ${CMAKE_INSTALL_PREFIX}/examples")
endif()

message(STATUS "Configured ${CMAKE_CURRENT_LIST_DIR} with 5 core examples")
message(STATUS "Build all examples with: make examples_all")