# Rune Caster 문서화

이 디렉토리는 Rune Caster 프로젝트의 API 문서화와 관련된 파일들을 포함합니다.

## 📚 문서 구조

```
docs/
├── README.md           # 이 파일
├── mainpage.md         # Doxygen 메인 페이지
├── style.css           # 사용자 정의 CSS 스타일
└── api/                # 생성된 API 문서 (빌드 후)
    └── html/
        └── index.html  # API 문서 메인 페이지
```

## 🛠️ 문서 생성 방법

### 1. 사전 요구사항

- **Doxygen** 설치
  ```bash
  # Windows (Chocolatey)
  choco install doxygen.install

  # Ubuntu/Debian
  sudo apt install doxygen

  # macOS (Homebrew)
  brew install doxygen
  ```

- **Graphviz** (선택사항, 다이어그램 생성용)
  ```bash
  # Windows (Chocolatey)
  choco install graphviz

  # Ubuntu/Debian
  sudo apt install graphviz

  # macOS (Homebrew)
  brew install graphviz
  ```

### 2. CMake를 통한 문서 생성

```bash
# 프로젝트 루트에서
cd build_test  # 또는 빌드 디렉토리

# 문서 생성
cmake --build . --target docs

# 또는 직접 make 사용 (Linux/macOS)
make docs
```

### 3. 문서 보기

생성된 문서는 다음 경로에서 확인할 수 있습니다:
- `build_test/docs/html/index.html` (빌드 디렉토리 기준)

브라우저에서 해당 파일을 열어서 API 문서를 확인하세요.

## 📋 문서화 규칙

### Doxygen 주석 스타일

```cpp
/**
 * @brief 간단한 설명을 여기에 작성합니다.
 *
 * 더 자세한 설명은 여기에 작성합니다.
 * 여러 줄로 작성할 수 있습니다.
 *
 * @param param1 첫 번째 매개변수 설명
 * @param param2 두 번째 매개변수 설명
 * @return 반환값에 대한 설명
 *
 * @note 주의사항이나 참고사항
 * @warning 경고사항
 * @see 관련 함수나 클래스 참조
 *
 * @code
 * // 사용 예제
 * auto rune = Rune::from_utf8("A");
 * @endcode
 */
```

### 한글 문서화 지원

- **브리프 설명**: 한 문장으로 간결하게
- **상세 설명**: 필요시 한글로 자세히 설명
- **매개변수**: 타입과 용도를 명확히 설명
- **예제 코드**: 실제 사용법을 보여주는 코드 포함

### 코드 예제 작성

```cpp
/**
 * @brief Unicode 문자를 생성합니다.
 *
 * @code
 * // 다양한 방법으로 Rune 생성
 * auto korean = U'가'_rune;
 * auto english = Rune::from_utf8("A");
 * auto japanese = Rune(U'あ');
 *
 * // 언어별 속성 확인
 * if (korean.is_hangul()) {
 *     std::cout << "한글 문자입니다." << std::endl;
 * }
 * @endcode
 */
```

## 🎨 스타일 커스터마이징

- `style.css` 파일을 수정하여 문서의 모양을 변경할 수 있습니다.
- 한글 폰트 최적화 및 다크 테마 지원이 포함되어 있습니다.
- 반응형 디자인으로 모바일에서도 잘 보입니다.

## 📖 문서 섹션

생성되는 API 문서는 다음 섹션들을 포함합니다:

1. **메인 페이지**: 프로젝트 개요 및 빠른 시작 가이드
2. **클래스 목록**: 모든 클래스와 구조체
3. **네임스페이스**: 조직화된 네임스페이스별 내용
4. **파일 목록**: 헤더 파일과 소스 파일
5. **함수 목록**: 모든 공개 함수와 메서드
6. **예제**: 실제 사용 예제들

## 🔧 고급 설정

### 추가 Doxygen 태그

```cpp
/**
 * @addtogroup rune_system Rune 시스템
 * @{
 */

/**
 * @brief Rune 클래스
 * @ingroup rune_system
 */
class Rune {
    // ...
};

/** @} */ // rune_system 그룹 끝
```

### 수식 지원

```cpp
/**
 * @brief 텍스트 유사도를 계산합니다.
 *
 * 유사도는 다음 공식으로 계산됩니다:
 *
 * \f[
 * similarity = \frac{|A \cap B|}{|A \cup B|}
 * \f]
 *
 * 여기서 A와 B는 문자 집합입니다.
 */
```

## 📝 기여 가이드

문서화에 기여할 때는 다음 사항들을 확인해주세요:

1. **완전성**: 모든 공개 API가 문서화되었는지 확인
2. **정확성**: 설명과 실제 구현이 일치하는지 확인
3. **예제**: 실제 동작하는 예제 코드 포함
4. **언어**: 한글과 영어 혼용 시 일관성 유지
5. **스타일**: 기존 문서화 스타일과 일관성 유지

## 🚀 자동화

향후 CI/CD 파이프라인에서 자동으로 문서를 생성하고 배포할 예정입니다:

- GitHub Actions를 통한 자동 문서 생성
- GitHub Pages 또는 별도 호스팅을 통한 온라인 문서 제공
- Pull Request시 문서 미리보기 기능

---

**도움이 필요하시면** GitHub Issues에 문의해주세요! 🙋‍♂️
