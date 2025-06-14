# 🚀 Rune Caster 빠른 시작 가이드

환영합니다! 이 가이드는 Rune Caster를 빠르게 시작할 수 있도록 도와드립니다.

## 📋 사전 요구사항

- **C++ 컴파일러**: C++20 지원 (GCC 9+, Clang 10+, MSVC 19.20+)
- **CMake**: 3.20 이상
- **Git**: 소스 코드 클론용

## 🔧 설치

### 1. 저장소 클론

```bash
git clone https://github.com/yourusername/rune-caster.git
cd rune-caster
```

### 2. 빌드

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### 3. 테스트 실행

```bash
ctest --verbose
```

## 💡 기본 사용법

### Rune 기본 사용

```cpp
#include "rune_caster/rune.hpp"
using namespace rune_caster;

// 다양한 언어의 문자 생성
Rune korean(U'가');
Rune english(U'A');
Rune japanese(U'あ');

// 언어 자동 감지
std::cout << "Korean: " << static_cast<int>(korean.language()) << std::endl;

// UTF-8 변환
std::cout << "UTF-8: " << korean.to_utf8() << std::endl;

// 문자 분류
std::cout << "Is vowel: " << english.is_vowel() << std::endl;
```

### RuneSequence 사용

```cpp
#include "rune_caster/rune_sequence.hpp"

// UTF-8 문자열에서 생성
auto sequence = RuneSequence::from_utf8("안녕하세요 Hello こんにちは");

// STL 스타일 이터레이션
for (const auto& rune : sequence) {
    std::cout << "Char: " << rune.to_utf8() << std::endl;
}
```

### Spell 텍스트 처리

```cpp
#include "rune_caster/spell.hpp"

// 파이프 연산자로 텍스트 처리
std::string text = "  Hello   WORLD!  ";
auto result = text | spell::normalize_whitespace() 
                  | spell::lowercase() 
                  | spell::trim();
// 결과: "hello world!"
```

### Caster 파이프라인

```cpp
#include "rune_caster/caster.hpp"

// 복잡한 텍스트 처리 파이프라인
auto pipeline = make_caster()
    .cast(spell::normalize_whitespace())
    .cast(spell::titlecase())
    .cast(spell::unicode_normalize(NormalizationForm::NFC));

std::string result = pipeline("  hello   world  ");
// 결과: "Hello World"
```

## 🔍 고급 기능

### 다국어 지원

```cpp
// 한글 모음 인식
Rune hangul_vowel(U'ㅏ');
assert(hangul_vowel.is_vowel() == true);

// 일본어 모음 인식
Rune hiragana_vowel(U'あ');
assert(hiragana_vowel.is_vowel() == true);
```

### 커스텀 Spell 작성

```cpp
auto custom_spell = [](const RuneSequence& input) {
    RuneSequence output;
    for (const auto& rune : input) {
        if (rune.is_digit()) {
            output.push_back(Rune(U'_'));
        } else {
            output.push_back(rune);
        }
    }
    return output;
};

// 사용
std::string result = "Hello123World" | custom_spell;
// 결과: "Hello___World"
```

## 📖 다음 단계

- [API 문서](https://yourusername.github.io/rune-caster) 살펴보기
- [예제 프로그램](examples/) 실행해보기
- [개발 가이드](docs/development-guide.md) 읽기

## 🛠️ 문제 해결

### 일반적인 문제

1. **빌드 실패**: C++20 지원 컴파일러를 사용하세요
2. **의존성 오류**: ICU 라이브러리 설치를 권장합니다
3. **테스트 실패**: `ctest --verbose`로 자세한 정보 확인

### 도움말

- [이슈 트래커](https://github.com/yourusername/rune-caster/issues)
- [토론 포럼](https://github.com/yourusername/rune-caster/discussions)
- [위키](https://github.com/yourusername/rune-caster/wiki)

---

🎉 **축하합니다!** Rune Caster를 성공적으로 설정했습니다.