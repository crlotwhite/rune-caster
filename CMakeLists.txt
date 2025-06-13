cmake_minimum_required(VERSION 3.20)
project(RuneCaster
    VERSION 1.0.0
    DESCRIPTION "Modern C++ Text Processing Framework"
    LANGUAGES CXX
)

# C++20 표준 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# C++20 모듈 지원은 향후 추가 예정
# set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
# set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

# 빌드 옵션
option(RUNE_CASTER_ENABLE_TESTS "Enable testing" ON)
option(RUNE_CASTER_ENABLE_BENCHMARKS "Enable benchmarking" OFF)
option(RUNE_CASTER_ENABLE_EXAMPLES "Enable examples" ON)
option(RUNE_CASTER_ENABLE_STATIC_ANALYSIS "Enable static analysis tools" OFF)

# 의존성 관리 옵션
option(RUNE_CASTER_USE_SYSTEM_FMT "Use system-installed fmt library" OFF)
option(RUNE_CASTER_USE_SYSTEM_SPDLOG "Use system-installed spdlog library" OFF)
option(RUNE_CASTER_USE_SYSTEM_ICU "Use system-installed ICU library" ON)
option(RUNE_CASTER_ENABLE_ICU "Enable ICU Unicode support" ON)
option(RUNE_CASTER_ENABLE_FMT "Enable fmt formatting library" ON)
option(RUNE_CASTER_ENABLE_SPDLOG "Enable spdlog logging library" ON)

# FetchContent 모듈 포함
include(FetchContent)

# 의존성 관리 함수
function(fetch_dependency name git_repo git_tag)
    string(TOUPPER ${name} name_upper)
    if(RUNE_CASTER_USE_SYSTEM_${name_upper})
        find_package(${name} QUIET)
        if(${name}_FOUND)
            message(STATUS "Using system ${name}")
            return()
        else()
            message(STATUS "System ${name} not found, falling back to FetchContent")
        endif()
    endif()

    message(STATUS "Fetching ${name} from ${git_repo} (${git_tag})")
    FetchContent_Declare(
        ${name}
        GIT_REPOSITORY ${git_repo}
        GIT_TAG ${git_tag}
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(${name})
endfunction()

# === FetchContent 의존성 ===

# fmt 라이브러리 (포맷팅)
if(RUNE_CASTER_ENABLE_FMT)
    fetch_dependency(fmt "https://github.com/fmtlib/fmt.git" "10.2.1")
endif()

# spdlog 라이브러리 (로깅)
if(RUNE_CASTER_ENABLE_SPDLOG)
    fetch_dependency(spdlog "https://github.com/gabime/spdlog.git" "v1.12.0")
endif()

# === 시스템 의존성 (복잡한 라이브러리들) ===

# ICU 라이브러리 옵션 설정
option(RUNE_CASTER_USE_FETCHCONTENT_ICU "Use FetchContent to download ICU instead of system ICU" OFF)
option(RUNE_CASTER_USE_UNI_ALGO "Use uni-algo as a lightweight Unicode alternative to ICU" ON)

# ICU 라이브러리 (유니코드 지원) - 복잡하므로 시스템 설치 우선
if(RUNE_CASTER_ENABLE_ICU AND RUNE_CASTER_USE_SYSTEM_ICU)
    find_package(PkgConfig QUIET)
    find_package(ICU COMPONENTS uc i18n data QUIET)
    if(NOT ICU_FOUND)
        message(STATUS "ICU not found via CMake, trying pkg-config")
        if(PkgConfig_FOUND)
            pkg_check_modules(ICU QUIET icu-uc icu-i18n)
        endif()
    endif()

    if(NOT ICU_FOUND AND NOT ICU_LIBRARIES)
        message(STATUS "ICU not found. Using built-in Unicode fallback implementation.")
        message(STATUS "For full Unicode support, install ICU:")
        message(STATUS "  Windows: vcpkg install icu")
        message(STATUS "  Ubuntu: sudo apt install libicu-dev")
        message(STATUS "  macOS: brew install icu4c")
        message(STATUS "Alternative: Set RUNE_CASTER_USE_FETCHCONTENT_ICU=ON or RUNE_CASTER_USE_UNI_ALGO=ON")
    endif()
endif()

# FetchContent ICU 옵션 (실험적)
if(RUNE_CASTER_ENABLE_ICU AND RUNE_CASTER_USE_FETCHCONTENT_ICU AND NOT ICU_FOUND)
    message(STATUS "🚀 Attempting to download ICU via FetchContent (experimental)...")
    message(WARNING "ICU FetchContent is experimental and may take 10-20 minutes to build!")

    # 방법 1: viaduck/icu-cmake 래퍼 사용 (권장)
    FetchContent_Declare(
        icu-cmake
        GIT_REPOSITORY https://github.com/viaduck/icu-cmake.git
        GIT_TAG        master  # 또는 특정 릴리즈 태그
        GIT_SHALLOW    ON
    )

    # ICU 빌드 설정 (빌드 시간 단축)
    set(BUILD_ICU ON CACHE BOOL "Build ICU from source")
    set(ICU_BUILD_VERSION "71.1" CACHE STRING "ICU version to build")

    FetchContent_MakeAvailable(icu-cmake)

    if(TARGET icu)
        set(ICU_FOUND TRUE)
        set(ICU_LIBRARIES icu)
        message(STATUS "✅ ICU successfully downloaded and configured via FetchContent")
    else()
        message(WARNING "❌ ICU FetchContent failed. Falling back to limited Unicode support.")
    endif()
endif()

# 대안: uni-algo 경량 Unicode 라이브러리 (권장)
if(RUNE_CASTER_USE_UNI_ALGO AND NOT ICU_FOUND)
    message(STATUS "🎯 Using uni-algo as lightweight Unicode alternative...")

    FetchContent_Declare(
        uni-algo
        GIT_REPOSITORY https://github.com/uni-algo/uni-algo.git
        GIT_TAG        v1.2.0  # 최신 안정 버전
        GIT_SHALLOW    ON
    )

    # uni-algo 헤더 전용 모드 활성화 (컴파일 시간 단축)
    set(UNI_ALGO_HEADER_ONLY ON CACHE INTERNAL "")

    FetchContent_MakeAvailable(uni-algo)

    if(TARGET uni-algo::uni-algo)
        set(UNI_ALGO_FOUND TRUE)
        message(STATUS "✅ uni-algo successfully downloaded and configured")
        # target_compile_definitions는 라이브러리 연결 섹션에서 처리
    endif()
endif()

# 선택적 의존성 (시스템 설치)
find_package(TBB QUIET)  # 병렬 처리
if(RUNE_CASTER_ENABLE_BENCHMARKS)
    find_package(benchmark QUIET)  # 성능 측정
endif()

# 메인 라이브러리 생성
add_library(rune_caster)

# 컴파일러별 플래그 설정
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(rune_caster PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
        $<$<CONFIG:Debug>:-g -O0 -DDEBUG>
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(rune_caster PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
        $<$<CONFIG:Debug>:-g -O0 -DDEBUG>
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(rune_caster PRIVATE
        /W4
        /utf-8  # UTF-8 인코딩 강제 (C4819 경고 해결)
        /wd4819 # C4819 경고 억제 (문자 인코딩 경고)
        $<$<CONFIG:Release>:/O2 /DNDEBUG>
        $<$<CONFIG:Debug>:/Od /DDEBUG>
    )
endif()

# 소스 파일들
target_sources(rune_caster
    PRIVATE
        src/rune/rune.cpp
        src/rune/rune_sequence.cpp
        src/spell/whitespace_normalizer.cpp
)

# 헤더 포함 경로
target_include_directories(rune_caster
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src
)

# === 의존성 연결 ===

# fmt 라이브러리 연결
if(RUNE_CASTER_ENABLE_FMT)
    if(TARGET fmt::fmt)
        target_link_libraries(rune_caster PRIVATE fmt::fmt)
        target_compile_definitions(rune_caster PRIVATE RUNE_CASTER_HAS_FMT)
        message(STATUS "fmt library linked successfully")
    else()
        message(STATUS "fmt not available, using fallback formatting")
    endif()
endif()

# spdlog 라이브러리 연결
if(RUNE_CASTER_ENABLE_SPDLOG)
    if(TARGET spdlog::spdlog)
        target_link_libraries(rune_caster PRIVATE spdlog::spdlog)
        target_compile_definitions(rune_caster PRIVATE RUNE_CASTER_HAS_SPDLOG)
        message(STATUS "spdlog library linked successfully")
    else()
        message(STATUS "spdlog not available, using basic logging")
    endif()
endif()

# ICU 또는 uni-algo 라이브러리 연결
if(RUNE_CASTER_ENABLE_ICU)
    if(ICU_FOUND AND TARGET icu)
        # FetchContent ICU (viaduck/icu-cmake)
        target_link_libraries(rune_caster PUBLIC icu)
        target_compile_definitions(rune_caster PRIVATE RUNE_CASTER_HAS_ICU)
        message(STATUS "FetchContent ICU library linked successfully")
    elseif(ICU_FOUND)
        # 시스템 ICU
        target_link_libraries(rune_caster PUBLIC ICU::uc ICU::i18n ICU::data)
        target_compile_definitions(rune_caster PRIVATE RUNE_CASTER_HAS_ICU)
        message(STATUS "System ICU library linked successfully")
    elseif(ICU_LIBRARIES)
        # pkg-config ICU
        target_link_libraries(rune_caster PUBLIC ${ICU_LIBRARIES})
        target_include_directories(rune_caster SYSTEM PUBLIC ${ICU_INCLUDE_DIRS})
        target_compile_definitions(rune_caster PRIVATE RUNE_CASTER_HAS_ICU)
        message(STATUS "ICU library (pkg-config) linked successfully")
    elseif(UNI_ALGO_FOUND)
        # uni-algo 대안
        target_link_libraries(rune_caster PUBLIC uni-algo::uni-algo)
        target_compile_definitions(rune_caster PRIVATE RUNE_CASTER_HAS_UNI_ALGO)
        message(STATUS "uni-algo Unicode library linked successfully")
    else()
        message(STATUS "No Unicode library available, using basic fallback support")
    endif()
endif()

# TBB 라이브러리 연결 (선택적)
if(TBB_FOUND)
    target_link_libraries(rune_caster PRIVATE TBB::tbb)
    target_compile_definitions(rune_caster PRIVATE RUNE_CASTER_HAS_TBB)
    message(STATUS "TBB library linked successfully")
endif()

# 하위 디렉토리 추가
if(RUNE_CASTER_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests/unit)
endif()

if(RUNE_CASTER_ENABLE_EXAMPLES)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt)
        add_subdirectory(examples)
    endif()
endif()

# === 설치 설정 (개발 단계용 간소화) ===
# 참고: FetchContent를 사용하는 경우, install은 복잡할 수 있음
# 현재는 개발/테스트 단계이므로 기본적인 install만 제공

include(GNUInstallDirs)

# 기본 라이브러리 설치 (export 없이)
install(TARGETS rune_caster
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 헤더 파일 설치
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# TODO: 향후 안정화되면 완전한 CMake 패키지 설정 추가
# - RuneCasterTargets.cmake
# - RuneCasterConfig.cmake
# - RuneCasterConfigVersion.cmake