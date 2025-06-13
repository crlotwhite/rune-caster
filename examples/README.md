# 🔮 Rune Caster 예제 코드

이 디렉토리는 Rune Caster 라이브러리의 핵심 기능을 시연하는 예제 코드들을 포함합니다. 모든 예제는 현대적인 C++20 기능을 활용하여 작성되었습니다.

## 📚 예제 목록

### 🌟 핵심 예제들 (5개)

#### 1. `basic_usage.cpp` - Rune 클래스 기초
**목적**: Rune 클래스의 기본 기능과 다국어 문자 처리
**주요 내용**:
- Rune 객체 생성 (ASCII, 한글, 일본어, 이모지)
- 문자 분류 및 속성 확인 (letter, vowel, consonant, whitespace, digit)
- UTF-8 변환 및 인코딩
- Factory 메서드 활용
- 음성학적 표현 설정
- 객체 비교 연산

```cpp
// 예시 코드
Rune korean_char(U'한');
std::cout << "Korean: " << korean_char.to_utf8() << std::endl;
std::cout << "Is letter: " << korean_char.is_letter() << std::endl;
```

#### 2. `sequence_usage.cpp` - RuneSequence 컨테이너
**목적**: RuneSequence 클래스의 컨테이너 기능과 STL 호환성
**주요 내용**:
- RuneSequence 생성 및 초기화
- STL 컨테이너 인터페이스 (push_back, front, back, iterators)
- 다국어 텍스트 처리
- 부분 문자열 추출 및 조작
- 시퀀스 병합 및 비교
- 실용적인 텍스트 통계 분석

```cpp
// 예시 코드
auto text = RuneSequence::from_utf8("Hello, 안녕하세요! 🌍");
std::cout << "Length: " << text.size() << " runes" << std::endl;
auto substr = text.substr(0, 5);
```

#### 3. `spell_demo.cpp` - 종합 스펠 시스템
**목적**: Spell 시스템의 모든 주요 기능을 포괄적으로 시연
**주요 내용**:
- 기본 스펠 클래스들 (WhitespaceNormalizer, CaseConverter, UnicodeNormalizer)
- 스펠 팩토리 함수들 (`spell::whitespace()`, `spell::lowercase()` 등)
- 스펠 조합 및 체이닝
- 캐스터 파이프라인 시스템
- 다국어 텍스트 처리
- 타입 안전성 및 성능 데모

```cpp
// 예시 코드
auto result = make_caster(RuneSequence::from_utf8("  Hello WORLD  "))
             .cast(spell::WhitespaceNormalizer{})
             .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
             .result();
```

#### 4. `case_converter_usage.cpp` - 케이스 변환 전문
**목적**: 텍스트 케이스 변환의 모든 측면을 다루는 전문적인 예제
**주요 내용**:
- 대소문자 변환 (Lower, Upper, Title)
- 다국어 텍스트에서의 케이스 처리
- 파이프라인과의 통합
- 실용적인 사용 사례 (사용자명 정규화, URL slug 생성)
- 배치 처리 및 성능 최적화
- 특수 케이스 및 에지 케이스 처리

```cpp
// 예시 코드
auto normalized = make_caster(RuneSequence::from_utf8("  john   DOE  "))
                 .cast(spell::WhitespaceNormalizer{})
                 .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
                 .result();
```

#### 5. `caster_pipeline_demo.cpp` - 고급 파이프라인
**목적**: Caster 파이프라인 시스템의 고급 기능과 실용적 워크플로우
**주요 내용**:
- 현대적인 fluent interface 파이프라인
- 단계별 변환 시각화
- 다양한 파이프라인 패턴
- 배치 처리 시스템
- 실용적인 워크플로우 (사용자 입력, 검색어, DB 필드)
- C++20 move semantics 최적화
- 메모리 효율성 및 타입 안전성

```cpp
// 예시 코드
auto advanced_result = make_caster(RuneSequence::from_utf8(input))
                      .cast(spell::WhitespaceNormalizer{true, true})
                      .cast(spell::UnicodeNormalizer{})
                      .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
                      .result();
```

## 🛠️ 빌드 및 실행

### CMake를 사용한 빌드

```bash
# 프로젝트 루트에서
mkdir build && cd build
cmake ..
make examples_all

# 또는 특정 예제만 빌드
make basic_usage
make spell_demo
```

### 예제 실행

```bash
# build 디렉토리에서
cd examples

# 각 예제 실행
./basic_usage
./sequence_usage
./spell_demo
./case_converter_usage
./caster_pipeline_demo
```

### Visual Studio (Windows)

1. Solution Explorer에서 예제 프로젝트 선택
2. 우클릭 → "Set as Startup Project"
3. F5 또는 Ctrl+F5로 실행
4. 출력: `build/examples/Debug/` 또는 `Release/`

## 🌟 학습 경로 추천

### 초보자
1. **basic_usage** - Rune 클래스 기초 이해
2. **sequence_usage** - 컨테이너 및 텍스트 조작
3. **case_converter_usage** - 실용적인 텍스트 변환

### 중급자
4. **spell_demo** - 전체 스펠 시스템 파악
5. **caster_pipeline_demo** - 고급 파이프라인 및 최적화

## 🔧 기술적 특징

### C++20 현대 기능
- **Concepts**: 컴파일 타임 타입 검증
- **Move Semantics**: 제로 카피 최적화
- **Perfect Forwarding**: 효율적인 파라미터 전달
- **Auto Type Deduction**: 타입 안전 변환

### 성능 최적화
- **Memory Efficiency**: Move semantics로 불필요한 복사 최소화
- **Compile-time Safety**: Concepts로 런타임 에러 방지
- **Pipeline Optimization**: 체인 연산에서 중간 할당 최소화

### 다국어 지원
- **Unicode Compliance**: UTF-8, UTF-16, UTF-32 완전 지원
- **Script Detection**: 자동 문자 체계 인식
- **Language Hints**: 언어별 최적화 힌트

## 🚀 실용적 활용

### 일반적인 사용 사례
- **텍스트 정규화**: 사용자 입력 데이터 정리
- **검색 최적화**: 검색어 전처리 및 정규화
- **데이터 표준화**: 데이터베이스 필드 형식 통일
- **다국어 처리**: 국제화 및 현지화 지원

### 성능이 중요한 경우
- **배치 처리**: 대량 텍스트 데이터 변환
- **실시간 처리**: 스트리밍 텍스트 변환
- **메모리 제약**: 임베디드 시스템에서의 텍스트 처리

## 📖 추가 자료

- [API 문서](../docs/html/index.html) - 완전한 API 레퍼런스
- [빠른 시작 가이드](../QUICK_START.md) - 프로젝트 시작하기
- [프로젝트 README](../README.md) - 전체 프로젝트 개요

## 🤝 기여하기

예제 개선이나 새로운 예제 추가를 원하시면:
1. 이슈를 먼저 등록해 주세요
2. 기존 예제 스타일을 따라 주세요
3. 충분한 주석과 설명을 포함해 주세요
4. 테스트를 통해 동작을 검증해 주세요

---

**💡 팁**: 각 예제는 독립적으로 실행할 수 있으며, 실제 프로덕션 코드에서 참조할 수 있는 베스트 프랙티스를 포함하고 있습니다.