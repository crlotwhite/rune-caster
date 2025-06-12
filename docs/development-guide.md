# Rune Caster - Development Guide

## 1. 개발 환경 설정

### 1.1 필수 요구사항

#### 컴파일러 지원
- **GCC 11+**: C++20 Modules, Concepts, Ranges 완전 지원
- **Clang 13+**: 최신 C++20 기능 지원
- **MSVC 19.29+**: Visual Studio 2019 16.11 이상

#### 플랫폼 지원
- **Linux**: Ubuntu 20.04+, Fedora 33+, Arch Linux
- **macOS**: macOS 11+ (Big Sur), Xcode 13+
- **Windows**: Windows 10 2004+, Visual Studio 2019+

#### CMake 버전
- **CMake 3.20+**: C++20 Modules 지원 필수

### 1.2 의존성 라이브러리

#### 핵심 의존성
```cmake
# CMakeLists.txt 예시
find_package(ICU 69.0 REQUIRED COMPONENTS uc i18n data)
find_package(fmt 8.0 REQUIRED)
find_package(spdlog 1.9 REQUIRED)

# 선택적 의존성
find_package(TBB QUIET)  # 병렬 처리
find_package(Benchmark QUIET)  # 성능 측정
find_package(GTest REQUIRED)  # 단위 테스트
```

#### 패키지 매니저별 설치

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential cmake git
sudo apt install libicu-dev libfmt-dev libspdlog-dev
sudo apt install libtbb-dev  # 선택적
sudo apt install libgtest-dev libgmock-dev
```

**macOS (Homebrew):**
```bash
brew install cmake git icu4c fmt spdlog
brew install tbb  # 선택적
brew install googletest
```

**Windows (vcpkg):**
```powershell
vcpkg install icu fmt spdlog
vcpkg install tbb  # 선택적
vcpkg install gtest
```

### 1.3 프로젝트 구조

```
rune-caster/
├── CMakeLists.txt                 # 메인 빌드 설정
├── cmake/
│   ├── modules/                   # CMake 모듈들
│   ├── toolchain/                 # 도구체인 설정
│   └── config.cmake.in            # 설정 템플릿
├── include/
│   └── rune_caster/              # 공개 헤더들
├── src/
│   ├── rune/                     # Rune 모듈 구현
│   ├── spell/                    # Spell 모듈 구현
│   ├── caster/                   # Caster 모듈 구현
│   ├── phonetic/                 # 음성학적 처리
│   ├── language/                 # 언어별 모듈들
│   └── utils/                    # 유틸리티
├── modules/                      # C++20 모듈 인터페이스
│   ├── rune_caster.cppm          # 메인 모듈
│   ├── rune.cppm                 # Rune 모듈
│   ├── spell.cppm                # Spell 모듈
│   ├── caster.cppm               # Caster 모듈
│   └── ...
├── tests/
│   ├── unit/                     # 단위 테스트
│   ├── integration/              # 통합 테스트
│   ├── benchmark/                # 성능 테스트
│   └── data/                     # 테스트 데이터
├── examples/                     # 사용 예제들
├── docs/                         # 문서
└── scripts/                      # 빌드/배포 스크립트
```

## 2. 빌드 시스템

### 2.1 CMake 설정

#### 메인 CMakeLists.txt
```cmake
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

# 모듈 지원 활성화
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

# 컴파일러별 플래그
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(rune_caster PRIVATE
        -fcoroutines -fmodules-ts -Wall -Wextra -Wpedantic
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(rune_caster PRIVATE
        -stdlib=libc++ -fmodules -Wall -Wextra -Wpedantic
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(rune_caster PRIVATE
        /std:c++20 /experimental:module /W4
    )
endif()

# 모듈 빌드
add_library(rune_caster)
target_sources(rune_caster
    PUBLIC
        FILE_SET CXX_MODULES FILES
            modules/rune_caster.cppm
            modules/rune.cppm
            modules/spell.cppm
            modules/caster.cppm
            modules/phonetic.cppm
            modules/language.cppm
    PRIVATE
        src/rune/rune.cpp
        src/rune/rune_string.cpp
        src/spell/cleaning.cpp
        src/spell/normalization.cpp
        src/caster/immediate.cpp
        src/phonetic/ipa.cpp
        src/language/korean.cpp
        src/language/english.cpp
)

# 의존성 연결
target_link_libraries(rune_caster
    PUBLIC
        ICU::uc ICU::i18n ICU::data
        fmt::fmt
        spdlog::spdlog
    PRIVATE
        $<$<TARGET_EXISTS:TBB::tbb>:TBB::tbb>
)
```

#### 모듈별 CMakeLists.txt
```cmake
# src/rune/CMakeLists.txt
add_library(rune_caster_rune)

target_sources(rune_caster_rune
    PUBLIC
        FILE_SET CXX_MODULES FILES
            ${PROJECT_SOURCE_DIR}/modules/rune.cppm
    PRIVATE
        rune.cpp
        rune_string.cpp
        rune_view.cpp
        unicode_utils.cpp
)

target_link_libraries(rune_caster_rune
    PUBLIC
        ICU::uc ICU::i18n
    PRIVATE
        fmt::fmt
)

# 성능 최적화
target_compile_options(rune_caster_rune PRIVATE
    $<$<CONFIG:Release>:-O3 -DNDEBUG -march=native>
    $<$<CONFIG:Debug>:-g -O0 -DDEBUG>
)
```

### 2.2 빌드 명령어

#### 기본 빌드
```bash
# 프로젝트 클론
git clone https://github.com/username/rune-caster.git
cd rune-caster

# 빌드 디렉토리 생성
mkdir build
cd build

# CMake 구성
cmake .. -DCMAKE_BUILD_TYPE=Release

# 빌드 실행
cmake --build . --parallel $(nproc)

# 테스트 실행
ctest --parallel $(nproc)
```

#### 고급 빌드 옵션
```bash
# 디버그 빌드
cmake .. -DCMAKE_BUILD_TYPE=Debug -DRUNE_CASTER_ENABLE_SANITIZERS=ON

# 성능 최적화 빌드
cmake .. -DCMAKE_BUILD_TYPE=Release -DRUNE_CASTER_ENABLE_LTO=ON -DRUNE_CASTER_ENABLE_SIMD=ON

# 정적 분석 포함
cmake .. -DRUNE_CASTER_ENABLE_CLANG_TIDY=ON -DRUNE_CASTER_ENABLE_CPPCHECK=ON

# 코드 커버리지
cmake .. -DCMAKE_BUILD_TYPE=Debug -DRUNE_CASTER_ENABLE_COVERAGE=ON
```

## 3. 코딩 표준

### 3.1 명명 규칙

#### 타입 및 클래스
```cpp
// PascalCase 사용
class RuneString;
struct PhoneticMapping;
enum class LanguageCode;

// 개념(Concept)도 PascalCase
template<typename T>
concept RuneLike = /* ... */;

// 타입 별칭은 PascalCase
using RuneContainer = std::vector<Rune>;
```

#### 변수 및 함수
```cpp
// snake_case 사용
void process_text(const RuneView& input);
auto language_code = detect_language(text);
bool is_valid_input = validate(data);

// 멤버 변수는 trailing underscore
class Rune {
private:
    char32_t codepoint_;
    LanguageCode language_hint_;
};
```

#### 상수 및 매크로
```cpp
// SCREAMING_SNAKE_CASE
constexpr std::size_t MAX_PHONEME_LENGTH = 32;
constexpr char32_t INVALID_CODEPOINT = 0xFFFFFFFF;

#define RUNE_CASTER_VERSION_MAJOR 1
```

#### 네임스페이스
```cpp
// snake_case, 짧고 명확하게
namespace runes {
    namespace spells {
        namespace korean {
            // 구현
        }
    }
}
```

### 3.2 코드 스타일

#### .clang-format 설정
```yaml
---
Language: Cpp
BasedOnStyle: Google
Standard: c++20

# 들여쓰기
IndentWidth: 4
UseTab: Never
IndentCaseLabels: true
IndentPPDirectives: BeforeHash

# 중괄호
BreakBeforeBraces: Attach
AllowShortFunctionsOnASingleLine: Inline
AllowShortIfStatementsOnASingleLine: Never
AllowShortLoopsOnASingleLine: false

# 라인 길이
ColumnLimit: 100
ReflowComments: true

# 정렬
AlignAfterOpenBracket: Align
AlignConsecutiveAssignments: false
AlignConsecutiveDeclarations: false
AlignOperands: true

# 공백
SpaceAfterCStyleCast: false
SpaceAfterTemplateKeyword: true
SpaceBeforeAssignmentOperators: true
SpaceBeforeParens: ControlStatements
SpaceInEmptyParentheses: false

# 헤더 순서
SortIncludes: true
IncludeBlocks: Regroup
IncludeCategories:
  - Regex: '^<.*\.h>'
    Priority: 1
  - Regex: '^<.*>'
    Priority: 2
  - Regex: '^".*"'
    Priority: 3
```

#### 모듈 스타일
```cpp
// modules/rune.cppm
export module rune_caster.rune;

// 표준 라이브러리는 import
import std;

// 다른 모듈들
import rune_caster.unicode;
import rune_caster.concepts;

// 전통적 헤더들 (필요한 경우만)
#include <icu/unicode/uchar.h>

// 네임스페이스는 export 내에서
export namespace runes {

class Rune {
public:
    // 공개 인터페이스
    constexpr Rune() noexcept = default;
    constexpr explicit Rune(char32_t codepoint) noexcept;

    // 속성 접근자는 const noexcept
    constexpr char32_t code_point() const noexcept { return codepoint_; }
    constexpr bool is_valid() const noexcept { return codepoint_ != INVALID_CODEPOINT; }

    // 변환 함수들
    std::string to_utf8() const;
    std::u16string to_utf16() const;
    std::u32string to_utf32() const;

    // 비교 연산자 (C++20 스타일)
    auto operator<=>(const Rune& other) const noexcept = default;

private:
    char32_t codepoint_ = INVALID_CODEPOINT;

    static constexpr char32_t INVALID_CODEPOINT = 0xFFFFFFFF;
};

} // namespace runes
```

### 3.3 문서화 표준

#### Doxygen 스타일
```cpp
/**
 * @brief 유니코드 문자를 표현하는 기본 클래스
 *
 * Rune 클래스는 단일 유니코드 코드포인트와 관련된 언어 힌트를
 * 포함하여 다국어 텍스트 처리의 기본 단위를 제공합니다.
 *
 * @note 이 클래스는 constexpr 생성과 연산을 지원하여 컴파일 타임
 *       최적화가 가능합니다.
 *
 * @example
 * @code
 * auto rune = Rune{U'가'};
 * if (rune.is_hangul()) {
 *     auto ipa = rune.to_ipa();
 *     // 음성학적 처리...
 * }
 * @endcode
 */
export class Rune {
public:
    /**
     * @brief 유니코드 코드포인트로 Rune을 생성합니다
     *
     * @param codepoint 유효한 유니코드 코드포인트 (0x0-0x10FFFF)
     * @param language_hint 언어 힌트 (선택적)
     *
     * @pre codepoint는 유효한 유니코드 범위 내에 있어야 함
     * @post is_valid() == true
     *
     * @exception std::invalid_argument 잘못된 코드포인트
     */
    constexpr explicit Rune(char32_t codepoint,
                           LanguageCode language_hint = LanguageCode::Unknown) noexcept;

    /**
     * @brief 이 룬이 한글 문자인지 확인합니다
     *
     * @return 한글 자모나 완성형 문자인 경우 true
     *
     * @complexity O(1)
     * @threadsafe Yes
     */
    constexpr bool is_hangul() const noexcept;
};
```

## 4. 테스트 전략

### 4.1 단위 테스트

#### Google Test 기반 테스트
```cpp
// tests/unit/rune_test.cpp
import rune_caster.rune;
import <gtest/gtest.h>;

using namespace runes;

class RuneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 테스트 설정
    }

    void TearDown() override {
        // 정리
    }
};

TEST_F(RuneTest, ConstructorValidCodepoint) {
    constexpr auto rune = Rune{U'A'};

    EXPECT_TRUE(rune.is_valid());
    EXPECT_EQ(rune.code_point(), U'A');
    EXPECT_TRUE(rune.is_latin());
    EXPECT_FALSE(rune.is_hangul());
}

TEST_F(RuneTest, ConstructorInvalidCodepoint) {
    EXPECT_THROW(Rune{0x110000}, std::invalid_argument);
}

TEST_F(RuneTest, HangulClassification) {
    constexpr auto hangul_jamo = Rune{U'ㄱ'};
    constexpr auto hangul_syllable = Rune{U'가'};
    constexpr auto latin = Rune{U'A'};

    EXPECT_TRUE(hangul_jamo.is_hangul());
    EXPECT_TRUE(hangul_syllable.is_hangul());
    EXPECT_FALSE(latin.is_hangul());
}

TEST_F(RuneTest, UTF8Conversion) {
    const auto rune = Rune{U'가'};
    const auto utf8 = rune.to_utf8();

    EXPECT_EQ(utf8, "가");
    EXPECT_EQ(utf8.length(), 3); // UTF-8에서 한글은 3바이트
}

// 성능 테스트
TEST_F(RuneTest, PerformanceIsHangul) {
    constexpr auto rune = Rune{U'가'};

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        volatile bool result = rune.is_hangul();
        (void)result;
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    EXPECT_LT(duration.count(), 10000); // 10ms 미만
}
```

#### 개념(Concept) 테스트
```cpp
// tests/unit/concepts_test.cpp
import rune_caster.concepts;
import <gtest/gtest.h>;

using namespace runes;

TEST(ConceptsTest, RuneLikeConcept) {
    static_assert(RuneLike<Rune>);
    static_assert(!RuneLike<int>);
    static_assert(!RuneLike<std::string>);
}

TEST(ConceptsTest, RuneRangeConcept) {
    static_assert(RuneRange<RuneString>);
    static_assert(RuneRange<std::vector<Rune>>);
    static_assert(!RuneRange<std::string>);
    static_assert(!RuneRange<std::vector<int>>);
}

TEST(ConceptsTest, SpellConcept) {
    static_assert(Spell<CleanWhitespace>);
    static_assert(Spell<NormalizeUnicode<>>);

    // 사용자 정의 Spell 테스트
    struct MockSpell {
        using input_type = RuneView;
        using output_type = RuneString;

        RuneString process(RuneView input) const {
            return RuneString(input);
        }
    };

    static_assert(Spell<MockSpell>);
}
```

### 4.2 통합 테스트

#### 파이프라인 테스트
```cpp
// tests/integration/pipeline_test.cpp
import rune_caster;
import <gtest/gtest.h>;

using namespace runes;

class PipelineTest : public ::testing::Test {
protected:
    RuneString test_text_ = "  Hello,   세계!  "_rs;
};

TEST_F(PipelineTest, BasicPipeline) {
    auto pipeline = spells::make_pipeline(
        spells::clean_whitespace(),
        spells::normalize_unicode(),
        spells::to_lower()
    );

    auto result = pipeline(test_text_);

    EXPECT_EQ(result.to_utf8(), "hello, 세계!");
}

TEST_F(PipelineTest, ConditionalProcessing) {
    auto conditional = spells::make_conditional(
        [](const auto& input) { return std::ranges::size(input) > 10; },
        spells::aggressive_cleanup(),
        spells::standard_cleanup()
    );

    RuneString short_text = "Hi"_rs;
    RuneString long_text = "This is a very long text that needs aggressive cleanup"_rs;

    auto short_result = conditional(short_text);
    auto long_result = conditional(long_text);

    // 결과 검증
    EXPECT_FALSE(short_result.empty());
    EXPECT_FALSE(long_result.empty());
}

TEST_F(PipelineTest, LanguageSpecificProcessing) {
    auto mixed_text = "Hello 안녕하세요 こんにちは"_rs;

    auto korean_only = mixed_text | views::filter_language<LanguageCode::Korean>;
    auto english_only = mixed_text | views::filter_language<LanguageCode::English>;

    // 언어별 필터링 검증
    EXPECT_TRUE(std::ranges::any_of(korean_only, [](const Rune& r) {
        return r.is_hangul();
    }));

    EXPECT_TRUE(std::ranges::any_of(english_only, [](const Rune& r) {
        return r.is_latin();
    }));
}
```

### 4.3 벤치마크 테스트

#### Google Benchmark 기반
```cpp
// tests/benchmark/rune_benchmark.cpp
import rune_caster.rune;
import <benchmark/benchmark.h>;

using namespace runes;

static void BM_RuneConstruction(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(Rune{U'가'});
    }
}
BENCHMARK(BM_RuneConstruction);

static void BM_RuneStringFromUTF8(benchmark::State& state) {
    const std::string utf8_text = "안녕하세요, 세계!";

    for (auto _ : state) {
        benchmark::DoNotOptimize(RuneString{utf8_text});
    }
}
BENCHMARK(BM_RuneStringFromUTF8);

static void BM_SpellProcessing(benchmark::State& state) {
    auto text = "  Hello,   world!   "_rs;
    auto spell = spells::clean_whitespace();

    for (auto _ : state) {
        benchmark::DoNotOptimize(spell(text));
    }
}
BENCHMARK(BM_SpellProcessing);

static void BM_PipelineProcessing(benchmark::State& state) {
    auto text = "  Hello, 세계! This is a test.  "_rs;
    auto pipeline = spells::make_pipeline(
        spells::clean_whitespace(),
        spells::normalize_unicode(),
        spells::to_lower()
    );

    for (auto _ : state) {
        benchmark::DoNotOptimize(pipeline(text));
    }
}
BENCHMARK(BM_PipelineProcessing);

// 메모리 사용량 벤치마크
static void BM_MemoryUsage(benchmark::State& state) {
    std::vector<RuneString> strings;
    strings.reserve(state.range(0));

    for (auto _ : state) {
        state.PauseTiming();
        strings.clear();
        state.ResumeTiming();

        for (int i = 0; i < state.range(0); ++i) {
            strings.emplace_back("테스트 문자열"_rs);
        }

        benchmark::DoNotOptimize(strings);
    }

    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_MemoryUsage)->Range(8, 8<<10)->Complexity();

BENCHMARK_MAIN();
```

## 5. 정적 분석 및 품질 관리

### 5.1 정적 분석 도구

#### .clang-tidy 설정
```yaml
---
# .clang-tidy
Checks: >
  *,
  -altera-*,
  -android-*,
  -fuchsia-*,
  -llvmlibc-*,
  -zircon-*,
  -readability-magic-numbers,
  -cppcoreguidelines-avoid-magic-numbers,
  -modernize-use-trailing-return-type

WarningsAsErrors: ''

CheckOptions:
  - key: readability-identifier-naming.ClassCase
    value: CamelCase
  - key: readability-identifier-naming.FunctionCase
    value: lower_case
  - key: readability-identifier-naming.VariableCase
    value: lower_case
  - key: readability-identifier-naming.ConstantCase
    value: UPPER_CASE
  - key: readability-identifier-naming.EnumCase
    value: CamelCase
  - key: readability-identifier-naming.EnumConstantCase
    value: CamelCase
  - key: readability-identifier-naming.MemberCase
    value: lower_case
  - key: readability-identifier-naming.MemberSuffix
    value: '_'
  - key: readability-identifier-naming.NamespaceCase
    value: lower_case
  - key: performance-move-const-arg.CheckTriviallyCopyableMove
    value: false
  - key: modernize-use-default-member-init.UseAssignment
    value: true
```

#### cppcheck 설정
```xml
<?xml version="1.0"?>
<!-- .cppcheck -->
<project version="1">
    <root name="."/>
    <builddir>build</builddir>
    <analyze-all-vs-configs>true</analyze-all-vs-configs>
    <check-headers>true</check-headers>
    <check-unused-templates>true</check-unused-templates>
    <max-ctu-depth>10</max-ctu-depth>
    <max-template-recursion>100</max-template-recursion>

    <paths>
        <dir name="src"/>
        <dir name="modules"/>
        <dir name="include"/>
    </paths>

    <exclude>
        <path name="build/"/>
        <path name="third_party/"/>
        <path name="tests/data/"/>
    </exclude>

    <libraries>
        <library>std</library>
        <library>posix</library>
    </libraries>

    <suppressions>
        <suppression files="*_test.cpp">unusedFunction</suppression>
        <suppression files="*_benchmark.cpp">unusedFunction</suppression>
    </suppressions>
</project>
```

### 5.2 CI/CD 파이프라인

#### GitHub Actions 설정
```yaml
# .github/workflows/ci.yml
name: CI

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main, develop ]

jobs:
  build-and-test:
    strategy:
      matrix:
        os: [ubuntu-22.04, macos-12, windows-2022]
        compiler: [gcc-11, clang-13, msvc-19.29]
        build_type: [Debug, Release]
        exclude:
          - os: ubuntu-22.04
            compiler: msvc-19.29
          - os: macos-12
            compiler: msvc-19.29
          - os: windows-2022
            compiler: gcc-11
          - os: windows-2022
            compiler: clang-13

    runs-on: ${{ matrix.os }}

    steps:
    - uses: actions/checkout@v3
      with:
        submodules: recursive

    - name: Install dependencies (Ubuntu)
      if: runner.os == 'Linux'
      run: |
        sudo apt update
        sudo apt install -y build-essential cmake ninja-build
        sudo apt install -y libicu-dev libfmt-dev libspdlog-dev libtbb-dev
        sudo apt install -y libgtest-dev libgmock-dev
        sudo apt install -y clang-tidy cppcheck

    - name: Install dependencies (macOS)
      if: runner.os == 'macOS'
      run: |
        brew install cmake ninja icu4c fmt spdlog tbb googletest
        brew install llvm

    - name: Install dependencies (Windows)
      if: runner.os == 'Windows'
      run: |
        vcpkg install icu fmt spdlog tbb gtest

    - name: Configure CMake
      run: |
        cmake -B build -G Ninja \
          -DCMAKE_BUILD_TYPE=${{ matrix.build_type }} \
          -DRUNE_CASTER_ENABLE_TESTS=ON \
          -DRUNE_CASTER_ENABLE_BENCHMARKS=ON \
          -DRUNE_CASTER_ENABLE_STATIC_ANALYSIS=ON

    - name: Build
      run: cmake --build build --parallel

    - name: Test
      run: |
        cd build
        ctest --parallel --output-on-failure

    - name: Static Analysis
      if: matrix.os == 'ubuntu-22.04' && matrix.compiler == 'clang-13'
      run: |
        clang-tidy --config-file=.clang-tidy src/**/*.cpp
        cppcheck --project=.cppcheck --error-exitcode=1

    - name: Upload coverage
      if: matrix.build_type == 'Debug' && matrix.os == 'ubuntu-22.04'
      uses: codecov/codecov-action@v3
      with:
        files: build/coverage.xml

  documentation:
    runs-on: ubuntu-22.04
    steps:
    - uses: actions/checkout@v3

    - name: Install Doxygen
      run: sudo apt install -y doxygen graphviz

    - name: Generate documentation
      run: doxygen Doxyfile

    - name: Deploy to GitHub Pages
      if: github.ref == 'refs/heads/main'
      uses: peaceiris/actions-gh-pages@v3
      with:
        github_token: ${{ secrets.GITHUB_TOKEN }}
        publish_dir: ./docs/html
```

## 6. 기여 가이드라인

### 6.1 브랜치 전략

#### Git Flow 기반
```
main
├── develop
│   ├── feature/rune-unicode-support
│   ├── feature/spell-korean-normalization
│   └── feature/phonetic-conversion
├── release/1.0.0
└── hotfix/memory-leak-fix
```

### 6.2 커밋 메시지 형식

```
type(scope): brief description

Detailed explanation of the change, including:
- Why the change was made
- What was changed
- Any breaking changes

Fixes #123
```

**타입:**
- `feat`: 새로운 기능
- `fix`: 버그 수정
- `docs`: 문서 변경
- `style`: 코드 스타일 변경
- `refactor`: 리팩토링
- `test`: 테스트 추가/수정
- `chore`: 빌드 프로세스나 도구 변경

### 6.3 코드 리뷰 체크리스트

- [ ] 코드가 프로젝트 스타일 가이드를 따르는가?
- [ ] 모든 공개 API에 문서가 있는가?
- [ ] 적절한 단위 테스트가 포함되어 있는가?
- [ ] 성능에 영향을 주는 변경사항인가?
- [ ] 메모리 누수나 다른 리소스 문제가 없는가?
- [ ] C++20 기능을 적절히 활용했는가?
- [ ] 컴파일러 경고가 없는가?
- [ ] 정적 분석 도구를 통과하는가?

이 개발 가이드를 통해 팀은 일관된 품질의 코드를 작성하고, C++20의 최신 기능을 효과적으로 활용할 수 있습니다.