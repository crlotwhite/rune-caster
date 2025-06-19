# 🤝 Rune Caster 기여 가이드

Rune Caster 프로젝트에 기여해주셔서 감사합니다! 이 가이드는 효과적인 기여 방법을 안내합니다.

## 📋 기여 유형

### 🐛 버그 리포트
- [이슈 템플릿](https://github.com/yourusername/rune-caster/issues/new?template=bug_report.md) 사용
- 재현 가능한 최소 예제 제공
- 환경 정보 포함 (OS, 컴파일러, CMake 버전)

### 💡 기능 제안
- [기능 요청 템플릿](https://github.com/yourusername/rune-caster/issues/new?template=feature_request.md) 사용
- 사용 사례와 예제 코드 제공
- 기존 API와의 호환성 고려

### 📖 문서 개선
- API 문서, 예제, 튜토리얼 개선
- 오타 수정, 명확성 향상
- 다국어 번역 (한국어, 일본어, 중국어)

### 🔧 코드 기여
- 버그 수정
- 새로운 기능 구현
- 성능 최적화
- 테스트 추가

## 🚀 개발 환경 설정

### 1. 저장소 포크 및 클론

```bash
# 1. GitHub에서 포크
# 2. 클론
git clone https://github.com/yourusername/rune-caster.git
cd rune-caster

# 3. 업스트림 추가
git remote add upstream https://github.com/original/rune-caster.git
```

### 2. 개발 의존성 설치

```bash
# macOS
brew install cmake ninja doxygen

# Ubuntu
sudo apt install cmake ninja-build doxygen graphviz

# Windows (Chocolatey)
choco install cmake ninja doxygen.install
```

### 3. 빌드 및 테스트

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTING=ON
cmake --build .
ctest --verbose
```

## 📝 코딩 스타일

### C++ 스타일 가이드

```cpp
// 1. 네임스페이스
namespace rune_caster {
namespace detail {
    // 내부 구현
}
}

// 2. 클래스 명명
class RuneSequence {          // PascalCase
public:
    void add_rune();          // snake_case 메서드
private:
    std::string data_;        // snake_case + trailing underscore
};

// 3. 상수
constexpr std::size_t MAX_BUFFER_SIZE = 1024;  // UPPER_SNAKE_CASE

// 4. 템플릿
template<typename T>
concept RuneLike = requires(T t) {           // concept 사용
    { t.codepoint() } -> std::convertible_to<char32_t>;
};
```

### 문서화 스타일

```cpp
/**
 * @brief UTF-8 문자열을 RuneSequence로 변환
 * @param utf8_text UTF-8 인코딩된 입력 문자열
 * @param language 선택적 언어 힌트
 * @return 변환된 RuneSequence 객체
 * @throws std::invalid_argument 잘못된 UTF-8 시퀀스
 * 
 * @example
 * ```cpp
 * auto seq = RuneSequence::from_utf8("Hello, 세계!");
 * assert(seq.size() == 9);
 * ```
 */
static RuneSequence from_utf8(std::string_view utf8_text, 
                              language::Code language = language::Code::Auto);
```

## 🧪 테스트 가이드라인

### 1. 단위 테스트 작성

```cpp
#include <gtest/gtest.h>
#include "rune_caster/rune.hpp"

TEST(RuneTest, KoreanVowelClassification) {
    // Given
    Rune hangul_a(U'ㅏ');
    
    // When & Then
    EXPECT_TRUE(hangul_a.is_vowel());
    EXPECT_FALSE(hangul_a.is_consonant());
    EXPECT_EQ(hangul_a.language(), language::Code::Korean);
}
```

### 2. 테스트 범위

- **100% 새로운 코드**: 모든 새로운 함수는 테스트 필수
- **90% 전체 코드**: `ctest`로 확인
- **경계값 테스트**: 빈 문자열, null, 극한값
- **다국어 테스트**: 최소 3개 언어 (영어, 한국어, 일본어)

### 3. 성능 테스트

```cpp
#include <benchmark/benchmark.h>

static void BM_RuneSequenceCreation(benchmark::State& state) {
    std::string text = "안녕하세요, Hello, こんにちは";
    for (auto _ : state) {
        auto seq = RuneSequence::from_utf8(text);
        benchmark::DoNotOptimize(seq);
    }
}
BENCHMARK(BM_RuneSequenceCreation);
```

## 🔄 기여 워크플로우

### 1. 브랜치 생성

```bash
git checkout -b feature/korean-vowel-support
# 또는
git checkout -b fix/utf8-parsing-bug
# 또는  
git checkout -b docs/api-improvements
```

### 2. 개발 사이클

```bash
# 개발
vim src/rune/rune.cpp

# 테스트
cmake --build build
cd build && ctest

# 커밋
git add .
git commit -m "feat: add Korean vowel recognition

- Add Unicode range U+314F-U+3163 for Hangul vowels
- Update is_vowel() method with Korean support
- Add comprehensive test cases
- Fixes #123"
```

### 3. 커밋 메시지 규칙

```
<type>(<scope>): <subject>

<body>

<footer>
```

**타입:**
- `feat`: 새로운 기능
- `fix`: 버그 수정
- `docs`: 문서만 변경
- `style`: 코드 스타일 (포맷팅, 세미콜론 등)
- `refactor`: 기능 변경 없는 코드 리팩토링
- `test`: 테스트 추가 또는 수정
- `perf`: 성능 개선
- `ci`: CI/CD 설정 변경

**예제:**
```
feat(rune): add comprehensive multi-language vowel support

- Add Korean vowel recognition (U+314F-U+3163)
- Add Japanese hiragana vowels (あいうえお)
- Add Japanese katakana vowels (アイウエオ)
- Update documentation with examples
- Add performance benchmarks

Fixes #123
Closes #124
```

### 4. Pull Request

```bash
git push origin feature/korean-vowel-support
```

**PR 체크리스트:**
- [ ] 모든 테스트 통과
- [ ] 코드 스타일 준수
- [ ] 문서 업데이트
- [ ] 커밋 메시지 규칙 준수
- [ ] Breaking change 여부 명시

## 🏗️ 아키텍처 가이드라인

### 모듈 구조

```
rune_caster/
├── rune/          # 핵심 문자 처리
├── spell/         # 텍스트 변환 파이프라인  
├── language/      # 언어 감지 및 처리
└── unicode/       # Unicode 표준 구현
```

### 성능 고려사항

1. **Zero-cost abstractions**: 런타임 오버헤드 없는 추상화
2. **Memory efficiency**: 불필요한 할당 최소화
3. **Cache locality**: 연속적인 메모리 접근 패턴
4. **SIMD 활용**: 벡터화 가능한 연산 설계

### API 설계 원칙

1. **Consistent**: 일관된 명명 규칙
2. **Composable**: 조합 가능한 인터페이스
3. **Type-safe**: 컴파일 타임 타입 안전성
4. **STL-compatible**: 표준 라이브러리와 호환

## 🚦 CI/CD 파이프라인

### 자동 검사

- **빌드 테스트**: GCC, Clang, MSVC
- **플랫폼 테스트**: Linux, macOS, Windows
- **코드 품질**: clang-tidy, cppcheck
- **문서 생성**: Doxygen 자동 배포
- **성능 회귀**: 벤치마크 비교

### 릴리스 프로세스

1. **Feature freeze**: 새 기능 중단
2. **Beta testing**: 커뮤니티 테스트
3. **Release candidate**: 최종 검증
4. **Stable release**: 정식 릴리스

## 🆘 도움말

### 문의 채널

- 💬 [GitHub Discussions](https://github.com/yourusername/rune-caster/discussions)
- 🐛 [Issue Tracker](https://github.com/yourusername/rune-caster/issues)
- 📧 Email: maintainers@rune-caster.org
- 💭 [Discord](https://discord.gg/rune-caster)

### 유용한 자료

- [C++20 Reference](https://en.cppreference.com/w/cpp/20)
- [Unicode Standard](https://unicode.org/standard/standard.html)
- [Google Test Documentation](https://google.github.io/googletest/)
- [CMake Best Practices](https://cliutils.gitlab.io/modern-cmake/)

---

**🎉 기여해주셔서 감사합니다!** 여러분의 기여가 Rune Caster를 더 나은 라이브러리로 만듭니다. 
