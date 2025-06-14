# Rune Caster

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![CMake](https://img.shields.io/badge/CMake-3.20%2B-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](https://github.com/yourusername/rune-caster)
[![Documentation](https://img.shields.io/badge/docs-GitHub%20Pages-brightgreen.svg)](https://yourusername.github.io/rune-caster)
[![Build Status](https://github.com/yourusername/rune-caster/workflows/📚%20Documentation/badge.svg)](https://github.com/yourusername/rune-caster/actions)

<div align="center">
  <img src="https://github.com/crlotwhite/rune-caster/blob/main/docs/logo.png?raw=true" width="480" height="480"/>
</div>

> **Modern C++ Text Processing Framework**
>
> STL에서 영감을 받은 직관적이고 고성능의 다국어 텍스트 처리 및 언어 간 변환 프레임워크

## 🌟 주요 특징

- **🔤 Rune 시스템**: 유니코드 문자와 문자열을 다루는 STL 스타일 컨테이너
- **⚡ Zero-cost Abstractions**: 런타임 오버헤드 없는 고성능 처리
- **🌍 다국어 지원**: 한국어, 영어, 일본어 등 Unicode 텍스트 처리
- **🎯 STL 호환**: 익숙한 컨테이너-알고리즘-이터레이터 패턴
- **🔧 C++20**: Modern C++ 기능을 적극 활용한 타입 안전한 API

## 📋 현재 구현 상태

### 🚧 개발 초기 단계 (진행 중)
- **프로젝트 구조**: CMake 빌드 시스템, 디렉토리 구조 완성
- **문서화**: 완전한 설계 문서 (PRD, 기술 아키텍처, API 설계)
- **헤더 파일**: 인터페이스 정의 (컴파일 오류 수정 필요)

### ⚠️ 알려진 이슈
- **컴파일 오류**: 현재 빌드 시 문법 오류 발생
- **인코딩 문제**: Windows 한국어 환경에서 UTF-8 처리 이슈
- **의존성 오류**: 클래스 간 참조 문제

### 🎯 즉시 필요한 작업
1. **기본 컴파일 문제 해결**
2. **Rune 클래스 기본 구현**
3. **Unicode 처리 시스템 구현**
4. **RuneSequence 컨테이너 구현**

### 🔮 향후 계획
- **Spell 시스템**: 텍스트 처리 알고리즘 (정규화, 정제, 필터링)
- **Caster 파이프라인**: 함수형 체이닝과 병렬 처리
- **Posterior Phonetic Gram**: 언어 간 음성학적 변환
- **다국어 모듈**: 언어별 특화 처리 규칙

## 🚀 빠른 시작

### 전제 조건

- **C++ 컴파일러**: C++20 지원 (GCC 9+, Clang 10+, MSVC 19.20+)
- **CMake**: 3.20 이상
- **Git**: 소스 코드 클론용

### 의존성 (선택사항)

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install libicu-dev libfmt-dev libspdlog-dev

# macOS (Homebrew)
brew install icu4c fmt spdlog

# Windows (vcpkg)
vcpkg install icu fmt spdlog
```

> **참고**: 의존성이 없어도 기본 기능은 작동하지만, ICU 설치를 권장합니다.

## 🔨 빌드 방법

### 1. 소스 코드 클론

```bash
git clone https://github.com/yourusername/rune-caster.git
cd rune-caster
```

### 2. 빌드 디렉토리 생성

```bash
mkdir build
cd build
```

### 3. CMake 설정

```bash
# 기본 빌드
cmake ..

# Release 모드
cmake -DCMAKE_BUILD_TYPE=Release ..

# 옵션 설정
cmake -DRUNE_CASTER_ENABLE_TESTS=ON \
      -DRUNE_CASTER_ENABLE_EXAMPLES=ON \
      -DCMAKE_BUILD_TYPE=Debug ..
```

### 4. 컴파일

```bash
# Windows (Visual Studio)
cmake --build . --config Release

# Linux/macOS
make -j$(nproc)

# 또는 ninja 사용 시
ninja
```

### 5. 설치 (선택사항)

```bash
sudo cmake --install .
```

## 📚 문서

### 🌐 온라인 문서
- **[API Documentation](https://yourusername.github.io/rune-caster)**: Doxygen으로 생성된 상세한 API 문서
- **[빠른 시작 가이드](QUICK_START.md)**: 프로젝트 시작을 위한 단계별 가이드
- **[보안 설정 가이드](docs/SECURITY_SETUP.md)**: GitHub Code Scanning 및 보안 스캔 설정

### 📖 로컬 문서 생성

```bash
# 문서 생성 (Doxygen 필요)
cmake -DRUNE_CASTER_ENABLE_DOCS=ON ..
cmake --build . --target docs

# 브라우저에서 열기
open docs/html/index.html  # macOS
xdg-open docs/html/index.html  # Linux
start docs/html/index.html  # Windows
```

### 🤖 자동 배포
- **GitHub Actions**: 코드 변경 시 자동으로 문서가 업데이트됩니다
- **GitHub Pages**: `https://yourusername.github.io/rune-caster`에서 접근 가능

## 📖 사용 예제

### 기본 Rune 사용법

```cpp
#include "rune_caster/rune.hpp"
#include <iostream>

using namespace rune_caster;

int main() {
    // 다양한 언어의 문자 생성
    Rune korean(U'가');
    Rune english(U'A');
    Rune japanese(U'あ');

    // 언어 자동 감지
    std::cout << "Korean detected: "
              << static_cast<int>(korean.language()) << std::endl;

    // UTF-8 변환
    std::cout << "UTF-8: " << korean.to_utf8() << std::endl;

    // 문자 분류
    std::cout << "Is vowel: " << english.is_vowel() << std::endl;
    std::cout << "Is consonant: " << english.is_consonant() << std::endl;

    return 0;
}
```

### RuneSequence 사용법

```cpp
#include "rune_caster/rune_sequence.hpp"
#include <iostream>

using namespace rune_caster;

int main() {
    // UTF-8 문자열에서 생성
    auto sequence = RuneSequence::from_utf8("안녕하세요 Hello こんにちは");

    // STL 스타일 이터레이션
    for (const auto& rune : sequence) {
        std::cout << "Char: " << rune.to_utf8()
                  << ", Language: " << static_cast<int>(rune.language())
                  << std::endl;
    }

    // 부분 문자열
    auto substr = sequence.substr(0, 5);
    std::cout << "Substring: " << substr.to_utf8() << std::endl;

    return 0;
}
```

## 📁 프로젝트 구조

```
rune-caster/
├── 📄 CMakeLists.txt          # 메인 빌드 설정
├── 📁 include/                # 공개 헤더 파일
│   └── rune_caster/
│       ├── rune.hpp           # Rune 클래스
│       ├── rune_sequence.hpp  # RuneSequence 컨테이너
│       ├── language.hpp       # 언어 코드 정의
│       ├── unicode.hpp        # Unicode 유틸리티
│       ├── concepts.hpp       # C++20 Concepts
│       └── version.hpp        # 버전 정보
├── 📁 src/                   # 구현 파일
│   ├── rune/                 # ✅ Rune 시스템 (구현됨)
│   ├── spell/                # 🚧 Spell 알고리즘 (예정)
│   ├── caster/              # 🚧 Caster 파이프라인 (예정)
│   ├── language/            # 🚧 언어 모듈 (예정)
│   └── phonetic/            # 🚧 음성학적 변환 (예정)
├── 📁 examples/              # 사용 예제
│   ├── basic_usage.cpp       # 기본 사용법
│   └── sequence_usage.cpp    # 시퀀스 사용법
├── 📁 tests/                # 테스트 파일
│   ├── unit/                # 단위 테스트
│   ├── integration/         # 통합 테스트
│   └── benchmark/           # 성능 벤치마크
└── 📁 docs/                 # 문서
    ├── PRD.md               # 프로젝트 요구사항
    ├── api-design.md        # API 설계
    ├── technical-architecture.md  # 기술 아키텍처
    └── development-guide.md # 개발 가이드
```

## 🔧 CMake 빌드 옵션

| 옵션 | 기본값 | 설명 |
|------|--------|------|
| `RUNE_CASTER_ENABLE_TESTS` | `ON` | 단위 테스트 빌드 |
| `RUNE_CASTER_ENABLE_BENCHMARKS` | `OFF` | 성능 벤치마크 빌드 |
| `RUNE_CASTER_ENABLE_EXAMPLES` | `ON` | 예제 프로그램 빌드 |
| `RUNE_CASTER_ENABLE_STATIC_ANALYSIS` | `OFF` | 정적 분석 도구 |

### 사용 예시

```bash
# 모든 기능 활성화
cmake -DRUNE_CASTER_ENABLE_TESTS=ON \
      -DRUNE_CASTER_ENABLE_BENCHMARKS=ON \
      -DRUNE_CASTER_ENABLE_EXAMPLES=ON \
      -DCMAKE_BUILD_TYPE=Release ..

# 최소 빌드 (라이브러리만)
cmake -DRUNE_CASTER_ENABLE_TESTS=OFF \
      -DRUNE_CASTER_ENABLE_EXAMPLES=OFF ..
```

## 🐛 문제 해결

### 자주 발생하는 문제

**1. ICU 라이브러리를 찾을 수 없음**
```bash
# 해결: ICU 설치 경로 명시
cmake -DICU_ROOT=/usr/local ..
```

**2. C++20 컴파일러 지원 부족**
```bash
# 해결: 최신 컴파일러 사용
sudo apt install gcc-10 g++-10
export CXX=g++-10
```

**3. Windows에서 UTF-8 출력 문제**
```cpp
// 해결: 콘솔 인코딩 설정
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
_setmode(_fileno(stdout), _O_U8TEXT);
#endif
```

## 📚 문서

- **[API 설계](docs/api-design.md)**: 상세한 API 문서
- **[기술 아키텍처](docs/technical-architecture.md)**: 시스템 설계 문서
- **[개발 가이드](docs/development-guide.md)**: 기여자를 위한 가이드
- **[프로젝트 요구사항](docs/PRD.md)**: 전체 프로젝트 로드맵

## 🤝 기여하기

1. **Fork** 이 저장소
2. **Feature 브랜치** 생성 (`git checkout -b feature/amazing-feature`)
3. **변경사항 커밋** (`git commit -m 'Add amazing feature'`)
4. **브랜치에 Push** (`git push origin feature/amazing-feature`)
5. **Pull Request** 생성

자세한 기여 가이드는 [CONTRIBUTING.md](CONTRIBUTING.md)를 참조하세요.

## 📄 라이선스

이 프로젝트는 [MIT 라이선스](LICENSE) 하에 배포됩니다.

## 🙏 감사의 말

- **STL**: 설계 철학과 API 패턴에 영감
- **ICU**: 강력한 유니코드 지원
- **C++20**: 현대적인 언어 기능들

---

⭐ **이 프로젝트가 유용하다면 Star를 눌러주세요!**
