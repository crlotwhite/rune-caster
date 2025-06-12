# 🚀 Rune Caster 5분 빠른 시작

> **현재 상태**: ✅ **완전히 작동하는 상태** - 바로 빌드하고 사용할 수 있습니다!

## ⚡ 초간단 시작 (Windows)

```powershell
# 1. 프로젝트 클론
git clone <your-repository-url>
cd rune-caster

# 2. 빌드 (자동 의존성 다운로드 포함)
mkdir build && cd build
cmake ..
cmake --build . --config Release

# 3. 예제 실행 - 바로 작동됩니다!
.\examples\Release\basic_usage.exe
.\examples\Release\sequence_usage.exe
```

## 🎯 즉시 사용 가능한 예제

### 기본 문자 처리
```cpp
#include <rune_caster/rune.hpp>
using namespace rune_caster;

// 한국어 문자 처리
Rune korean{U'안'};
std::cout << "Language: " << static_cast<int>(korean.language()) << std::endl;
std::cout << "UTF-8: " << korean.to_utf8() << std::endl;

// 영어 문자 처리
Rune english{U'A'};
std::cout << "Is vowel: " << english.is_vowel() << std::endl;
std::cout << "Is letter: " << english.is_letter() << std::endl;
```

### 텍스트 시퀀스 처리
```cpp
#include <rune_caster/rune_sequence.hpp>
using namespace rune_caster;

// 다국어 텍스트 처리
auto sequence = RuneSequence::from_utf8("Hello, 안녕하세요! こんにちは");
std::cout << "Length: " << sequence.size() << std::endl;
std::cout << "Primary language: " << static_cast<int>(sequence.primary_language()) << std::endl;

// STL 스타일 반복자 사용
for (const auto& rune : sequence) {
    std::cout << "Codepoint: " << rune.codepoint() << std::endl;
}
```

## ✅ 확인된 기능들

| 기능 | 상태 | 설명 |
|------|------|------|
| **한국어 처리** | ✅ 완전 지원 | 한글 자모, 완성형 문자 인식 |
| **영어 처리** | ✅ 완전 지원 | 모음/자음 분류, 대소문자 구분 |
| **일본어 처리** | ✅ 완전 지원 | 히라가나, 가타카나, 한자 지원 |
| **UTF-8 변환** | ✅ 완전 지원 | 양방향 UTF-8 ↔ UTF-32 변환 |
| **언어 자동 감지** | ✅ 완전 지원 | 문자별 언어 코드 자동 할당 |
| **음성 표현** | ✅ 완전 지원 | Phoneme 저장 및 관리 |
| **STL 호환성** | ✅ 완전 지원 | 표준 컨테이너/알고리즘 사용 가능 |

## 📦 자동 설치되는 의존성

프로젝트에서 자동으로 다운로드하고 빌드하는 라이브러리들:
- **fmt 10.2.1** - 현대적 C++ 포맷팅
- **spdlog 1.12.0** - 고성능 로깅
- **내장 Unicode 구현** - ICU 없이도 완전 동작

## 🔧 요구사항

- **Windows 10/11** (테스트됨)
- **Visual Studio 2022** (Community Edition도 가능)
- **CMake 3.20+**
- **Git** (의존성 자동 다운로드용)

## 📁 빌드 결과물

성공적인 빌드 후 생성되는 파일들:
```
build/
├── Release/
│   └── rune_caster.lib          # 메인 라이브러리 (96KB)
├── examples/Release/
│   ├── basic_usage.exe          # 기본 사용 예제
│   └── sequence_usage.exe       # 시퀀스 사용 예제
└── _deps/                       # 자동 다운로드된 의존성들
    ├── fmt-build/Release/fmt.lib
    └── spdlog-build/Release/spdlog.lib
```

## 🎮 인터랙티브 데모

빌드 후 바로 실행해볼 수 있는 예제들:

### basic_usage.exe 실행 결과:
```
=== Rune Caster Basic Usage Example ===
Version: 1.0.0
Description: Modern C++ Text Processing Framework

1. Creating basic Runes:
  Latin 'A': codepoint=65, language=2
  Hangul '가': codepoint=44032, language=1
  Hiragana 'あ': codepoint=12354, language=3

2. Character classification:
  'A' is_letter: 1
  'A' is_vowel: 1
  'A' is_consonant: 0
```

### sequence_usage.exe 실행 결과:
```
=== Rune Caster Sequence Usage Example ===
1. Creating RuneSequences:
  "Hello, 안녕하세요?" size: 13
  Korean sequence primary language: 1

2. STL Container Operations:
  Built sequence size: 5
  Characters in sequence: 72 101 108 108 111
```

## 🚀 다음 단계

1. **프로젝트에 통합**:
   ```cmake
   # 당신의 CMakeLists.txt에 추가
   add_subdirectory(rune-caster)
   target_link_libraries(your_project PRIVATE rune_caster)
   ```

2. **고급 기능 탐색**:
   - 음성학적 표현 (Phoneme) 활용
   - 다국어 텍스트 정규화
   - 언어별 특수 처리 로직

3. **문서 확인**:
   - `docs/api-design.md` - 상세 API 가이드
   - `docs/technical-architecture.md` - 기술 아키텍처
   - `docs/DEPENDENCIES.md` - 의존성 관리 가이드

## 🆘 문제 해결

### 빌드 실패 시:
```powershell
# 이전 빌드 정리 후 재시도
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
mkdir build && cd build
cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON
cmake --build . --config Release --verbose
```

### 여전히 문제가 있다면:
- Visual Studio Developer Command Prompt 사용
- `cmake --version`으로 3.20+ 확인
- Git이 PATH에 있는지 확인

---

**🎉 축하합니다!** 이제 현대적인 C++로 다국어 텍스트 처리를 시작할 수 있습니다!