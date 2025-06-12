# 📦 Rune Caster 의존성 관리

> **FetchContent 기반 자동 의존성 관리**
> 별도 도구 설치 없이 CMake만으로 모든 의존성 자동 설치

## 🎯 의존성 관리 전략

**Rune Caster는 FetchContent를 채택했습니다:**

### ✅ FetchContent 선택 이유

1. **🚀 Zero Setup**: 별도 도구 설치 불필요
2. **🔒 버전 고정**: CMakeLists.txt에서 정확한 버전 관리
3. **🌍 크로스 플랫폼**: Windows, Linux, macOS 동일하게 동작
4. **👥 기여자 친화적**: `git clone` 후 바로 빌드 가능
5. **🔄 CI/CD 최적화**: GitHub Actions에서 별도 설정 없이 동작

### 📊 vs. vcpkg 비교

| 특징 | **FetchContent** | vcpkg |
|------|------------------|-------|
| 설치 요구사항 | CMake만 | vcpkg 도구 설치 |
| 버전 관리 | 프로젝트별 명시적 | 시스템 전역 |
| 빌드 속도 | 느림 (소스 컴파일) | 빠름 (바이너리) |
| 프로젝트 독립성 | 완전 독립 | 시스템 의존 |
| CI/CD 복잡성 | 간단 | 설정 필요 |
| 적합한 프로젝트 | 개발 초기, 적은 의존성 | 복잡한 의존성, 큰 프로젝트 |

## 🔧 의존성 설정 옵션

### 기본 옵션

```cmake
# 의존성 활성화/비활성화
option(RUNE_CASTER_ENABLE_FMT "Enable fmt formatting library" ON)
option(RUNE_CASTER_ENABLE_SPDLOG "Enable spdlog logging library" ON)
option(RUNE_CASTER_ENABLE_ICU "Enable ICU Unicode support" ON)

# 시스템 라이브러리 우선 사용
option(RUNE_CASTER_USE_SYSTEM_FMT "Use system-installed fmt library" OFF)
option(RUNE_CASTER_USE_SYSTEM_SPDLOG "Use system-installed spdlog library" OFF)
option(RUNE_CASTER_USE_SYSTEM_ICU "Use system-installed ICU library" ON)
```

### 📋 현재 지원 의존성

| 라이브러리 | 용도 | 기본값 | FetchContent | 시스템 설치 |
|------------|------|--------|-------------|-------------|
| **fmt** | 문자열 포맷팅 | ON | ✅ v10.2.1 | 선택적 |
| **spdlog** | 구조화된 로깅 | ON | ✅ v1.12.0 | 선택적 |
| **ICU** | Unicode 처리 | ON | ❌ 복잡함 | 권장 |
| **TBB** | 병렬 처리 | OFF | ❌ 시스템만 | 선택적 |

## 🚀 사용 방법

### 1. 기본 빌드 (권장)

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

**자동으로 설치되는 것들:**
- fmt 10.2.1 (GitHub에서 자동 다운로드)
- spdlog 1.12.0 (GitHub에서 자동 다운로드)
- ICU 검색 시도 (시스템 설치 권장)

### 2. 의존성 비활성화

```bash
# fmt 없이 빌드 (기본 포맷팅 사용)
cmake -DRUNE_CASTER_ENABLE_FMT=OFF ..

# 로깅 라이브러리 없이 빌드
cmake -DRUNE_CASTER_ENABLE_SPDLOG=OFF ..

# ICU 없이 빌드 (제한된 Unicode 지원)
cmake -DRUNE_CASTER_ENABLE_ICU=OFF ..
```

### 3. 시스템 라이브러리 우선 사용

```bash
# 시스템 설치된 fmt 사용 (있으면)
cmake -DRUNE_CASTER_USE_SYSTEM_FMT=ON ..

# 모든 라이브러리를 시스템에서 찾기
cmake -DRUNE_CASTER_USE_SYSTEM_FMT=ON \
      -DRUNE_CASTER_USE_SYSTEM_SPDLOG=ON \
      -DRUNE_CASTER_USE_SYSTEM_ICU=ON ..
```

### 4. 최소 의존성 빌드

```bash
# 외부 의존성 없이 빌드
cmake -DRUNE_CASTER_ENABLE_FMT=OFF \
      -DRUNE_CASTER_ENABLE_SPDLOG=OFF \
      -DRUNE_CASTER_ENABLE_ICU=OFF ..
```

## 🔍 의존성 상태 확인

빌드 시 다음과 같은 메시지를 확인할 수 있습니다:

```
-- Fetching fmt from https://github.com/fmtlib/fmt.git (10.2.1)
-- fmt library linked successfully
-- Fetching spdlog from https://github.com/gabime/spdlog.git (v1.12.0)
-- spdlog library linked successfully
-- ICU not available, using fallback Unicode support
```

## 🌍 플랫폼별 권장 설정

### Windows (Visual Studio)

```bash
# 기본 설정 (추천)
cmake ..

# 또는 ICU 설치 후
# vcpkg install icu
cmake -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake ..
```

### Ubuntu/Debian

```bash
# ICU 설치 (권장)
sudo apt update
sudo apt install libicu-dev

# 빌드
cmake ..
```

### macOS

```bash
# ICU 설치 (권장)
brew install icu4c

# ICU 경로 설정이 필요할 수 있음
export PKG_CONFIG_PATH="/opt/homebrew/lib/pkgconfig:$PKG_CONFIG_PATH"
cmake ..
```

## 🐛 문제 해결

### 자주 발생하는 문제

**1. 네트워크 연결 오류**
```bash
# 오프라인 빌드 (시스템 라이브러리만 사용)
cmake -DRUNE_CASTER_USE_SYSTEM_FMT=ON \
      -DRUNE_CASTER_USE_SYSTEM_SPDLOG=ON ..
```

**2. FetchContent 캐시 문제**
```bash
# 캐시 정리 후 재빌드
rm -rf build/_deps
cmake --build . --target clean
cmake --build .
```

**3. ICU 라이브러리 찾기 실패**
```bash
# ICU 설치 경로 명시 (macOS)
cmake -DICU_ROOT=/opt/homebrew ..

# 또는 pkg-config 경로 설정
export PKG_CONFIG_PATH="/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH"
```

## 🎛️ 고급 설정

### 1. 특정 버전 사용

현재 `CMakeLists.txt`에서 버전을 수정할 수 있습니다:

```cmake
# fmt 9.1.0 사용하고 싶다면
fetch_dependency(fmt "https://github.com/fmtlib/fmt.git" "9.1.0")
```

### 2. 프록시 환경

```bash
# Git 프록시 설정
git config --global http.proxy http://proxy.company.com:8080
cmake ..
```

### 3. 오프라인 개발

```bash
# 첫 빌드로 의존성 캐시 생성
cmake ..
cmake --build .

# 이후 오프라인에서도 빌드 가능 (캐시 사용)
```

## 📊 성능 최적화

### 빌드 시간 단축

```bash
# 병렬 빌드
cmake --build . -j$(nproc)

# 시스템 라이브러리 사용 (더 빠름)
cmake -DRUNE_CASTER_USE_SYSTEM_FMT=ON \
      -DRUNE_CASTER_USE_SYSTEM_SPDLOG=ON ..
```

### 디스크 사용량 최적화

```bash
# 의존성 최소화
cmake -DRUNE_CASTER_ENABLE_SPDLOG=OFF ..

# 빌드 후 임시 파일 정리
rm -rf build/_deps/*/src/*-build
```

## 📈 로드맵

### 향후 추가 예정 의존성

- **Google Test**: 단위 테스트 프레임워크
- **Google Benchmark**: 성능 측정 도구
- **nlohmann/json**: JSON 처리 (설정 파일용)
- **CLI11**: 명령줄 인터페이스

### 고려 중인 개선사항

- 의존성 다운로드 진행률 표시
- 선택적 FetchContent vs vcpkg 하이브리드 모드
- 컴파일 시간 최적화를 위한 Unity Build 지원

---

💡 **추천 설정**: 개발 중에는 기본 설정(`cmake ..`)을 사용하고, 프로덕션 배포 시에는 시스템 라이브러리를 활용하세요.

# 📦 의존성 관리 가이드

## 🎯 의존성 관리 전략

**Rune Caster**는 현대적인 **FetchContent 기반 의존성 관리**를 사용합니다.

### ✅ **자동 관리되는 의존성 (FetchContent)**
- **fmt** 10.2.1 - 현대적 포맷팅 라이브러리
- **spdlog** 1.12.0 - 고성능 로깅 라이브러리
- **uni-algo** 1.2.0 - 경량 Unicode 처리 라이브러리

### 🔧 **Unicode 지원 옵션**

Unicode 처리를 위해 여러 옵션을 제공합니다:

| 옵션 | 설명 | 권장도 | 특징 |
|------|------|--------|------|
| **uni-algo** | 경량 Unicode 라이브러리 | ⭐⭐⭐⭐⭐ | 헤더 전용, 빠른 빌드, 표준 준수 |
| **시스템 ICU** | 시스템 설치된 ICU | ⭐⭐⭐⭐ | 완전한 기능, 별도 설치 필요 |
| **FetchContent ICU** | 자동 다운로드 ICU | ⭐⭐ | Windows에서 복잡함 (patch 필요) |
| **fallback 구현** | 기본 내장 구현 | ⭐⭐ | 제한적 기능 |

## 🚀 **빠른 시작 (권장)**

### 1. **uni-algo 사용 (기본값)**
```bash
# 기본 설정으로 바로 빌드 가능
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### 2. **시스템 ICU 사용**
```bash
# Ubuntu/Debian
sudo apt install libicu-dev

# macOS
brew install icu4c

# Windows (vcpkg)
vcpkg install icu

# 빌드
mkdir build && cd build
cmake .. -DRUNE_CASTER_USE_UNI_ALGO=OFF
cmake --build . --config Release
```

## 🔬 **FetchContent ICU 실험 결과**

FetchContent로 ICU 설치를 시도한 결과:

### ✅ **성공한 부분**
- uni-algo 자동 다운로드 및 빌드 완전 성공
- viaduck/icu-cmake 래퍼 다운로드 성공

### ❌ **문제점**
- Windows에서 `patch` 유틸리티 필요
- 복잡한 플랫폼별 의존성
- 예상 빌드 시간: 10-20분

### 🎯 **권장사항**
1. **일반 사용**: `uni-algo` 사용 (기본값)
2. **고급 기능**: 시스템 ICU 설치
3. **FetchContent ICU**: Git Bash + patch 환경에서만 권장

## ⚙️ **고급 설정**

### CMake 옵션

```cmake
# Unicode 라이브러리 선택
option(RUNE_CASTER_USE_UNI_ALGO "Use uni-algo Unicode library" ON)
option(RUNE_CASTER_USE_FETCHCONTENT_ICU "Use FetchContent ICU (experimental)" OFF)
option(RUNE_CASTER_USE_SYSTEM_ICU "Use system ICU" ON)

# 다른 옵션들
option(RUNE_CASTER_ENABLE_FMT "Enable fmt library" ON)
option(RUNE_CASTER_ENABLE_SPDLOG "Enable spdlog library" ON)
```

### 사용 예제

```bash
# uni-algo만 사용
cmake .. -DRUNE_CASTER_USE_UNI_ALGO=ON

# 시스템 ICU만 사용
cmake .. -DRUNE_CASTER_USE_UNI_ALGO=OFF -DRUNE_CASTER_USE_SYSTEM_ICU=ON

# FetchContent ICU 시도 (실험적)
cmake .. -DRUNE_CASTER_USE_FETCHCONTENT_ICU=ON -DRUNE_CASTER_USE_UNI_ALGO=OFF
```

## 🔍 **문제 해결**

### ICU 관련 문제

**문제**: `ICU not found`
```bash
# 해결 1: uni-algo 사용 (권장)
cmake .. -DRUNE_CASTER_USE_UNI_ALGO=ON

# 해결 2: 시스템 ICU 설치
# Ubuntu: sudo apt install libicu-dev
# macOS: brew install icu4c
# Windows: vcpkg install icu
```

**문제**: `Cannot find patch utility` (FetchContent ICU)
```bash
# 해결: Git Bash 사용 또는 uni-algo로 대체
cmake .. -DRUNE_CASTER_USE_UNI_ALGO=ON -DRUNE_CASTER_USE_FETCHCONTENT_ICU=OFF
```

### 빌드 시간 최적화

```bash
# 병렬 빌드 (권장)
cmake --build . --config Release --parallel 4

# 의존성 최소화
cmake .. -DRUNE_CASTER_ENABLE_EXAMPLES=OFF -DRUNE_CASTER_ENABLE_TESTS=OFF
```

## 📊 **성능 비교**

| 라이브러리 | 빌드 시간 | 메모리 사용 | 기능 완성도 | 플랫폼 호환성 |
|------------|-----------|-------------|-------------|---------------|
| uni-algo | ~30초 | 낮음 | 높음 | 완벽 |
| 시스템 ICU | 즉시 | 보통 | 최고 | 좋음 |
| FetchContent ICU | 10-20분 | 높음 | 최고 | 복잡 |
| fallback | 즉시 | 최소 | 기본 | 완벽 |

## 🎁 **추가 정보**

- **uni-algo 문서**: https://github.com/uni-algo/uni-algo
- **ICU 공식 사이트**: https://icu.unicode.org/
- **FetchContent 공식 문서**: https://cmake.org/cmake/help/latest/module/FetchContent.html

---

**💡 권장사항**: 대부분의 경우 `uni-algo`가 최적의 선택입니다. 빠르고, 안정적이며, 모든 플랫폼에서 잘 작동합니다.