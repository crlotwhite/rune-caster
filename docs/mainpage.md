# Rune Caster API Documentation {#mainpage}

## 🌟 개요

**Rune Caster**는 STL에서 영감을 받은 Modern C++ 텍스트 처리 프레임워크입니다. 직관적이고 고성능의 다국어 텍스트 처리 및 언어 간 변환 기능을 제공합니다.

## 🚀 주요 특징

- **🔤 Rune 시스템**: 유니코드 문자와 문자열을 다루는 STL 스타일 컨테이너
- **⚡ Zero-cost Abstractions**: 런타임 오버헤드 없는 고성능 처리
- **🌍 다국어 지원**: 한국어, 영어, 일본어 등 34개 언어 Unicode 텍스트 처리
- **🎯 STL 호환**: 익숙한 컨테이너-알고리즘-이터레이터 패턴
- **🔧 C++20**: Modern C++ 기능을 적극 활용한 타입 안전한 API

## 📚 핵심 컴포넌트

### 1. Rune 클래스
개별 Unicode 문자를 표현하는 기본 단위입니다.

```cpp
#include <rune_caster/rune.hpp>

// 다양한 방법으로 Rune 생성
auto korean = U'가'_rune;                          // 사용자 정의 리터럴
auto english = Rune::from_utf8("A");              // UTF-8에서 생성
auto japanese = Rune(U'あ');                      // 직접 생성

// 언어별 속성 확인 (constexpr 지원)
static_assert(korean.is_hangul());
static_assert(english.is_ascii());
static_assert(japanese.is_hiragana());
```

### 2. RuneString 클래스
Unicode 문자열을 다루는 STL 호환 컨테이너입니다.

```cpp
#include <rune_caster/rune_sequence.hpp>

// 다양한 방법으로 RuneString 생성
auto text = RuneString::from_utf8("안녕하세요 Hello こんにちは");
auto korean_part = text.substr(0, 5);
auto has_greeting = text.contains(U'안'_rune);

// STL 스타일 인터페이스
for (const auto& rune : text) {
    std::cout << "언어: " << static_cast<int>(rune.language()) << std::endl;
}
```

### 3. Spell 시스템
텍스트 변환 알고리즘을 제공합니다.

```cpp
#include <rune_caster/spell.hpp>

// Factory 함수를 통한 Spell 생성
auto normalized = text
    | spell::normalize_whitespace()      // 공백 정규화
    | spell::lowercase()                 // 소문자 변환
    | spell::unicode_nfc();              // Unicode 정규화

// Spell 조합
auto complex_spell = spell::compose(
    spell::normalize_whitespace(true, true),
    spell::lowercase()
);
```

### 4. Caster 파이프라인
함수형 스타일의 텍스트 처리 파이프라인을 제공합니다.

```cpp
#include <rune_caster/caster.hpp>
#include <rune_caster/spell.hpp>

using namespace rune_caster;

// Caster 스타일 파이프라인
auto result = make_caster(input_text)
    .cast(spell::normalize_whitespace())
    .cast(spell::lowercase())
    .cast(spell::unicode_nfc())
    .result();

// Pipe operator 스타일 (더 간단함)
auto result2 = input_text
    | spell::normalize_whitespace()
    | spell::lowercase();
```

## 🛠️ 빠른 시작

### 1. 기본 설정

```cpp
#include <rune_caster/rune.hpp>
#include <rune_caster/rune_sequence.hpp>
#include <rune_caster/spell.hpp>
#include <rune_caster/caster.hpp>

using namespace rune_caster;
```

### 2. 간단한 예제

```cpp
int main() {
    // 다국어 텍스트 생성
    auto text = RuneSequence::from_utf8("  Hello, 안녕하세요! こんにちは  ");

    // 텍스트 정제 (현재 API 사용)
    auto cleaned = text
        | spell::normalize_whitespace()  // 공백 정규화
        | spell::lowercase();            // 소문자 변환

    // 또는 Caster 사용
    auto result = make_caster(text)
        .cast(spell::normalize_whitespace())
        .cast(spell::trim())
        .cast(spell::lowercase())
        .result();

    // 결과 출력
    std::cout << "원본: " << text.to_utf8() << std::endl;
    std::cout << "정제: " << cleaned.to_utf8() << std::endl;
    std::cout << "주요 언어: " << static_cast<int>(text.primary_language()) << std::endl;

    return 0;
}
```

## 🎯 고급 기능

### constexpr 지원
컴파일 타임에 문자 속성을 확인할 수 있습니다.

```cpp
constexpr auto korean_char = U'가'_rune;
static_assert(korean_char.is_hangul());
static_assert(korean_char.language() == language::Code::Korean);
```

### C++20 Concepts
타입 안전성을 보장합니다.

```cpp
template<spell_for<RuneString> Spell>
auto process_text(const RuneString& text, Spell&& spell) {
    return spell(text);
}
```

### Ranges 지원
STL ranges와 완벽하게 호환됩니다.

```cpp
auto vowels = text
    | std::views::filter([](const Rune& r) { return r.is_vowel(); })
    | std::ranges::to<std::vector>();
```

## 📖 상세 문서

각 클래스와 함수의 상세한 사용법은 다음 섹션을 참조하세요:

- \ref rune_caster::Rune "Rune 클래스"
- \ref rune_caster::RuneString "RuneString 클래스"
- \ref rune_caster::spell "Spell 시스템"
- \ref rune_caster::caster "Caster 파이프라인"

## 🔧 빌드 요구사항

- **C++ 표준**: C++20 이상
- **컴파일러**: GCC 10+, Clang 10+, MSVC 19.29+
- **CMake**: 3.20 이상

## 📝 라이선스

MIT License

---

**Rune Caster**로 현대적이고 안전한 다국어 텍스트 처리를 시작해보세요! 🚀
