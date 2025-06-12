# Rune Caster - Technical Architecture

## 1. 아키텍처 개요

### 1.1 설계 철학
Rune Caster는 C++20의 최신 기능을 적극 활용하여 다음 원칙을 구현합니다:

- **Concepts 기반 타입 안전성**: 컴파일 타임에 타입 제약을 명확히 정의
- **Ranges 기반 함수형 처리**: 지연 평가와 파이프라인 체이닝을 통한 직관적 API
- **Modules 기반 모듈화**: 빠른 컴파일과 명확한 의존성 관리
- **Zero-cost Abstractions**: 런타임 오버헤드 없는 고수준 추상화

### 1.2 핵심 모듈 구조
```cpp
// C++20 Modules 구조
export module rune_caster;

export import rune_caster.rune;      // 기본 문자/문자열 타입
export import rune_caster.spell;     // 텍스트 처리 알고리즘
export import rune_caster.caster;    // 파이프라인 처리기
export import rune_caster.phonetic;  // 음성학적 변환
export import rune_caster.language;  // 언어별 특화 기능
```

## 2. Rune 시스템 (Container Layer)

### 2.1 Rune Concepts
```cpp
export module rune_caster.rune;

import std;

// 기본 Rune 컨셉 정의
template<typename T>
concept RuneLike = requires(T t) {
    typename T::value_type;
    { t.code_point() } -> std::convertible_to<char32_t>;
    { t.category() } -> std::convertible_to<unicode::Category>;
    { t.language() } -> std::convertible_to<language::Code>;
};

template<typename T>
concept RuneContainer = requires(T t) {
    typename T::iterator;
    typename T::const_iterator;
    typename T::value_type;
    requires RuneLike<typename T::value_type>;
    { t.begin() } -> std::input_iterator;
    { t.end() } -> std::input_iterator;
    { t.size() } -> std::convertible_to<std::size_t>;
};

template<typename T>
concept RuneRange = std::ranges::range<T> &&
                   RuneLike<std::ranges::range_value_t<T>>;
```

### 2.2 핵심 Rune 클래스들
```cpp
// 단일 유니코드 문자 표현
export class Rune {
private:
    char32_t code_point_;
    unicode::Category category_;
    language::Code lang_hint_;

public:
    constexpr Rune(char32_t cp) noexcept;
    constexpr Rune(char32_t cp, language::Code lang) noexcept;

    constexpr char32_t code_point() const noexcept { return code_point_; }
    constexpr unicode::Category category() const noexcept { return category_; }
    constexpr language::Code language() const noexcept { return lang_hint_; }

    // 언어별 속성 조회
    constexpr bool is_hangul() const noexcept;
    constexpr bool is_hiragana() const noexcept;
    constexpr bool is_katakana() const noexcept;
    constexpr bool is_kanji() const noexcept;

    // 음성학적 속성
    phonetic::IPA to_ipa() const;
    std::vector<phonetic::IPA> to_ipa_variants() const;
};

// 가변 길이 유니코드 문자열
export class RuneString {
private:
    std::vector<Rune> runes_;
    language::Code primary_language_;

public:
    using value_type = Rune;
    using iterator = std::vector<Rune>::iterator;
    using const_iterator = std::vector<Rune>::const_iterator;

    // STL 호환 인터페이스
    iterator begin() noexcept { return runes_.begin(); }
    iterator end() noexcept { return runes_.end(); }
    const_iterator begin() const noexcept { return runes_.begin(); }
    const_iterator end() const noexcept { return runes_.end(); }

    std::size_t size() const noexcept { return runes_.size(); }
    bool empty() const noexcept { return runes_.empty(); }

    // 생성자
    RuneString() = default;
    explicit RuneString(std::string_view utf8);
    explicit RuneString(std::u32string_view utf32);

    // 언어 감지 및 설정
    language::Code detect_language() const;
    void set_primary_language(language::Code lang);

    // 변환 함수
    std::string to_utf8() const;
    std::u32string to_utf32() const;

    // Ranges 지원
    auto as_range() const { return std::views::all(runes_); }
};

// 읽기 전용 뷰 (std::string_view와 유사)
export class RuneView {
private:
    const Rune* data_;
    std::size_t size_;

public:
    using value_type = Rune;
    using iterator = const Rune*;
    using const_iterator = const Rune*;

    constexpr RuneView() noexcept : data_(nullptr), size_(0) {}
    constexpr RuneView(const Rune* data, std::size_t size) noexcept
        : data_(data), size_(size) {}

    template<RuneContainer Container>
    constexpr RuneView(const Container& container) noexcept
        : data_(container.data()), size_(container.size()) {}

    constexpr const_iterator begin() const noexcept { return data_; }
    constexpr const_iterator end() const noexcept { return data_ + size_; }
    constexpr std::size_t size() const noexcept { return size_; }
    constexpr bool empty() const noexcept { return size_ == 0; }

    constexpr RuneView substr(std::size_t pos, std::size_t len = npos) const;
};
```

## 3. Spell 시스템 (Algorithm Layer)

### 3.1 Spell Concepts
```cpp
export module rune_caster.spell;

import std;
import rune_caster.rune;

// 기본 Spell 컨셉
template<typename T>
concept Spell = requires(T spell) {
    typename T::input_type;
    typename T::output_type;
    requires RuneRange<typename T::input_type>;
    requires RuneRange<typename T::output_type>;
    { spell(std::declval<typename T::input_type>()) }
        -> std::convertible_to<typename T::output_type>;
};

// 인플레이스 변환 Spell
template<typename T>
concept InPlaceSpell = Spell<T> && requires(T spell) {
    requires std::same_as<typename T::input_type, typename T::output_type>;
};

// 언어별 특화 Spell
template<typename T, language::Code Lang>
concept LanguageSpecificSpell = Spell<T> && requires {
    { T::supported_language } -> std::convertible_to<language::Code>;
    requires T::supported_language == Lang;
};

// 조합 가능한 Spell
template<typename T>
concept ComposableSpell = Spell<T> && requires(T spell) {
    { spell.compose_with(std::declval<Spell auto>()) } -> Spell;
};
```

### 3.2 핵심 Spell 구현
```cpp
// CRTP 기반 Spell 베이스 클래스
export template<typename Derived, RuneRange Input, RuneRange Output>
class SpellBase {
public:
    using input_type = Input;
    using output_type = Output;

    template<RuneRange R>
    requires std::convertible_to<R, Input>
    constexpr auto operator()(R&& range) const {
        return static_cast<const Derived*>(this)->process(std::forward<R>(range));
    }

    // Ranges 파이프라인 지원
    template<RuneRange R>
    friend constexpr auto operator|(R&& range, const SpellBase& spell) {
        return spell(std::forward<R>(range));
    }
};

// 공백 정제 Spell
export class CleanWhitespaceSpell : public SpellBase<CleanWhitespaceSpell, RuneView, RuneString> {
public:
    RuneString process(RuneView input) const {
        return input
            | std::views::filter([](const Rune& r) {
                return !unicode::is_whitespace(r.code_point()) ||
                       unicode::is_significant_whitespace(r.code_point());
              })
            | std::views::transform([](const Rune& r) {
                return unicode::is_whitespace(r.code_point()) ?
                       Rune(U' ') : r;
              })
            | std::ranges::to<RuneString>();
    }
};

// 유니코드 정규화 Spell
export template<unicode::NormalizationForm Form>
class NormalizeUnicodeSpell : public SpellBase<NormalizeUnicodeSpell<Form>, RuneView, RuneString> {
public:
    RuneString process(RuneView input) const {
        if constexpr (Form == unicode::NormalizationForm::NFC) {
            return normalize_nfc(input);
        } else if constexpr (Form == unicode::NormalizationForm::NFD) {
            return normalize_nfd(input);
        }
        // ... 기타 정규화 형식
    }

private:
    RuneString normalize_nfc(RuneView input) const;
    RuneString normalize_nfd(RuneView input) const;
};

// 언어 감지 Spell
export class DetectLanguageSpell : public SpellBase<DetectLanguageSpell, RuneView, language::DetectionResult> {
private:
    language::StatisticalModel model_;

public:
    explicit DetectLanguageSpell(const language::StatisticalModel& model) : model_(model) {}

    language::DetectionResult process(RuneView input) const {
        auto features = extract_language_features(input);
        return model_.classify(features);
    }

private:
    language::FeatureVector extract_language_features(RuneView input) const;
};

// 언어별 특화 Spell (한국어)
export class KoreanSpell : public SpellBase<KoreanSpell, RuneView, RuneString> {
public:
    static constexpr language::Code supported_language = language::Code::Korean;

    RuneString process(RuneView input) const {
        return input
            | std::views::transform([](const Rune& r) { return decompose_hangul(r); })
            | std::views::join
            | normalize_jamo_sequence()
            | recompose_hangul()
            | std::ranges::to<RuneString>();
    }

private:
    std::vector<Rune> decompose_hangul(const Rune& hangul) const;
    auto normalize_jamo_sequence() const;
    auto recompose_hangul() const;
};
```

### 3.3 고급 Spell 조합
```cpp
// Spell 조합 유틸리티
export template<Spell... Spells>
class SpellPipeline {
private:
    std::tuple<Spells...> spells_;

public:
    explicit SpellPipeline(Spells... spells) : spells_(spells...) {}

    template<RuneRange Input>
    auto process(Input&& input) const {
        return std::apply([&input](const auto&... spells) {
            return (input | ... | spells);
        }, spells_);
    }

    // 새로운 Spell 추가
    template<Spell NewSpell>
    auto add(NewSpell spell) const {
        return std::apply([spell](const auto&... existing) {
            return SpellPipeline(existing..., spell);
        }, spells_);
    }
};

// 팩토리 함수
export template<Spell... Spells>
constexpr auto make_pipeline(Spells... spells) {
    return SpellPipeline(spells...);
}

// 조건부 Spell 적용
export template<typename Predicate, Spell TrueSpell, Spell FalseSpell>
class ConditionalSpell {
private:
    Predicate predicate_;
    TrueSpell true_spell_;
    FalseSpell false_spell_;

public:
    template<RuneRange Input>
    auto process(Input&& input) const {
        if (predicate_(input)) {
            return true_spell_(std::forward<Input>(input));
        } else {
            return false_spell_(std::forward<Input>(input));
        }
    }
};
```

## 4. Caster 시스템 (Execution Layer)

### 4.1 Caster Concepts
```cpp
export module rune_caster.caster;

import std;
import rune_caster.rune;
import rune_caster.spell;

template<typename T>
concept Caster = requires(T caster) {
    typename T::input_type;
    typename T::output_type;
    { caster.process(std::declval<typename T::input_type>()) }
        -> std::convertible_to<typename T::output_type>;
};

template<typename T>
concept StreamCaster = Caster<T> && requires(T caster) {
    typename T::stream_type;
    { caster.process_stream(std::declval<typename T::stream_type>()) }
        -> std::ranges::range;
};

template<typename T>
concept AsyncCaster = Caster<T> && requires(T caster) {
    { caster.process_async(std::declval<typename T::input_type>()) }
        -> std::convertible_to<std::future<typename T::output_type>>;
};
```

### 4.2 기본 Caster 구현
```cpp
// 기본 텍스트 처리 Caster
export template<Spell SpellType>
class TextCaster {
private:
    SpellType spell_;

public:
    using input_type = typename SpellType::input_type;
    using output_type = typename SpellType::output_type;

    explicit TextCaster(SpellType spell) : spell_(std::move(spell)) {}

    output_type process(const input_type& input) const {
        return spell_(input);
    }

    // 파이프라인 연산자
    template<Spell OtherSpell>
    auto operator|(OtherSpell other_spell) const {
        return TextCaster(make_pipeline(spell_, other_spell));
    }
};

// 스트림 처리 Caster
export template<Spell SpellType>
class StreamCaster {
private:
    SpellType spell_;
    std::size_t chunk_size_;

public:
    using input_type = std::ranges::range auto;
    using output_type = std::ranges::range auto;

    explicit StreamCaster(SpellType spell, std::size_t chunk_size = 4096)
        : spell_(std::move(spell)), chunk_size_(chunk_size) {}

    template<std::ranges::range Input>
    auto process_stream(Input&& input) const {
        return input
            | std::views::chunk(chunk_size_)
            | std::views::transform([this](auto chunk) {
                return spell_(chunk | std::ranges::to<RuneString>());
              });
    }
};

// 병렬 처리 Caster
export template<Spell SpellType>
class ParallelCaster {
private:
    SpellType spell_;
    std::size_t thread_count_;

public:
    explicit ParallelCaster(SpellType spell, std::size_t threads = std::thread::hardware_concurrency())
        : spell_(std::move(spell)), thread_count_(threads) {}

    template<RuneRange Input>
    auto process(Input&& input) const {
        if (std::ranges::size(input) < 1000) {
            // 작은 입력은 단일 스레드로 처리
            return spell_(std::forward<Input>(input));
        }

        return process_parallel(std::forward<Input>(input));
    }

private:
    template<RuneRange Input>
    auto process_parallel(Input&& input) const;
};
```

### 4.3 메모리 최적화
```cpp
// 메모리 풀 기반 할당자
export template<typename T>
class RuneAllocator {
private:
    static inline std::unique_ptr<memory::Pool> pool_ =
        std::make_unique<memory::Pool>(sizeof(T) * 1024);

public:
    using value_type = T;

    T* allocate(std::size_t n) {
        if (n * sizeof(T) <= pool_->block_size()) {
            return static_cast<T*>(pool_->allocate(n * sizeof(T)));
        }
        return std::allocator<T>{}.allocate(n);
    }

    void deallocate(T* p, std::size_t n) {
        if (!pool_->owns(p)) {
            std::allocator<T>{}.deallocate(p, n);
        }
        // 풀 메모리는 자동 관리
    }
};

// 최적화된 RuneString
using OptimizedRuneString = std::vector<Rune, RuneAllocator<Rune>>;
```

## 5. Phonetic 시스템 (Posterior Phonetic Gram)

### 5.1 음성학적 표현
```cpp
export module rune_caster.phonetic;

import std;
import rune_caster.rune;

// IPA 음소 표현
export class IPA {
private:
    char32_t symbol_;
    phonetic::Manner manner_;
    phonetic::Place place_;
    phonetic::Voicing voicing_;

public:
    constexpr IPA(char32_t symbol) noexcept : symbol_(symbol) {
        auto features = phonetic::lookup_features(symbol);
        manner_ = features.manner;
        place_ = features.place;
        voicing_ = features.voicing;
    }

    constexpr char32_t symbol() const noexcept { return symbol_; }
    constexpr phonetic::Manner manner() const noexcept { return manner_; }
    constexpr phonetic::Place place() const noexcept { return place_; }
    constexpr phonetic::Voicing voicing() const noexcept { return voicing_; }

    // 음성학적 거리 계산
    double distance(const IPA& other) const noexcept;

    // 언어별 변환 가능성
    double conversion_probability(const IPA& target, language::Code target_lang) const;
};

// 음성학적 시퀀스
export class PhoneticSequence {
private:
    std::vector<IPA> sequence_;
    language::Code source_language_;

public:
    using iterator = std::vector<IPA>::iterator;
    using const_iterator = std::vector<IPA>::const_iterator;

    explicit PhoneticSequence(language::Code lang) : source_language_(lang) {}

    void add_phone(const IPA& phone) { sequence_.push_back(phone); }

    iterator begin() { return sequence_.begin(); }
    iterator end() { return sequence_.end(); }
    const_iterator begin() const { return sequence_.begin(); }
    const_iterator end() const { return sequence_.end(); }

    language::Code source_language() const { return source_language_; }
};
```

### 5.2 Posterior Phonetic Gram 엔진
```cpp
// 확률적 음성학적 변환 엔진
export class PhoneticGramEngine {
private:
    std::unordered_map<language::Code, phonetic::LanguageModel> models_;
    phonetic::TransitionMatrix transition_matrix_;

public:
    explicit PhoneticGramEngine(const std::filesystem::path& model_path);

    // 단일 언어 내 음성학적 분석
    PhoneticSequence analyze(RuneView text, language::Code lang) const;

    // 언어 간 변환
    std::vector<PhoneticConversionCandidate> convert(
        const PhoneticSequence& source,
        language::Code target_language,
        std::size_t max_candidates = 5
    ) const;

    // 베이지안 추론을 통한 확률 계산
    double calculate_conversion_probability(
        const PhoneticSequence& source,
        const PhoneticSequence& target,
        language::Code target_language
    ) const;

private:
    phonetic::LanguageModel load_language_model(language::Code lang) const;
    std::vector<PhoneticSequence> generate_candidates(
        const PhoneticSequence& source,
        language::Code target_language
    ) const;
};

// 변환 후보 결과
export struct PhoneticConversionCandidate {
    PhoneticSequence sequence;
    double probability;
    std::string romanized;
    RuneString native_script;

    bool operator<(const PhoneticConversionCandidate& other) const {
        return probability > other.probability; // 내림차순 정렬
    }
};
```

## 6. Language 시스템 (언어별 특화)

### 6.1 언어 모듈 인터페이스
```cpp
export module rune_caster.language;

import std;
import rune_caster.rune;
import rune_caster.spell;
import rune_caster.phonetic;

// 언어 모듈 컨셉
template<typename T>
concept LanguageModule = requires(T module) {
    { T::language_code } -> std::convertible_to<language::Code>;
    { module.detect(std::declval<RuneView>()) } -> std::convertible_to<double>;
    { module.normalize(std::declval<RuneView>()) } -> std::convertible_to<RuneString>;
    { module.to_phonetic(std::declval<RuneView>()) } -> std::convertible_to<PhoneticSequence>;
};

// 언어 모듈 베이스 클래스
export template<language::Code Lang>
class LanguageModuleBase {
public:
    static constexpr language::Code language_code = Lang;

    virtual ~LanguageModuleBase() = default;

    // 언어 감지 (0.0 ~ 1.0)
    virtual double detect(RuneView text) const = 0;

    // 언어별 정규화
    virtual RuneString normalize(RuneView text) const = 0;

    // 음성학적 변환
    virtual PhoneticSequence to_phonetic(RuneView text) const = 0;

    // 로마자 변환
    virtual std::string romanize(RuneView text) const = 0;

    // 언어별 특수 기능
    virtual std::optional<RuneString> apply_language_rules(RuneView text) const {
        return std::nullopt;
    }
};
```

### 6.2 한국어 모듈 구현
```cpp
// 한국어 특화 모듈
export class KoreanModule : public LanguageModuleBase<language::Code::Korean> {
private:
    hangul::JamoDecomposer decomposer_;
    hangul::JamoComposer composer_;
    korean::RomanizationSystem romanization_system_;

public:
    double detect(RuneView text) const override {
        auto hangul_ratio = std::ranges::count_if(text, [](const Rune& r) {
            return unicode::is_hangul(r.code_point());
        }) / static_cast<double>(text.size());

        return std::min(1.0, hangul_ratio * 1.2); // 한글 비율 기반 감지
    }

    RuneString normalize(RuneView text) const override {
        return text
            | std::views::transform([this](const Rune& r) {
                return decompose_and_normalize(r);
              })
            | std::views::join
            | compose_hangul()
            | std::ranges::to<RuneString>();
    }

    PhoneticSequence to_phonetic(RuneView text) const override {
        PhoneticSequence result(language::Code::Korean);

        for (const auto& rune : text) {
            auto ipa_phones = convert_hangul_to_ipa(rune);
            for (const auto& phone : ipa_phones) {
                result.add_phone(phone);
            }
        }

        return result;
    }

    std::string romanize(RuneView text) const override {
        return romanization_system_.convert(text);
    }

    // 한국어 특화 기능
    std::optional<RuneString> apply_language_rules(RuneView text) const override {
        // 높임말 처리, 불규칙 활용 정규화 등
        return korean::apply_honorific_normalization(text);
    }

private:
    std::vector<Rune> decompose_and_normalize(const Rune& hangul) const;
    auto compose_hangul() const;
    std::vector<IPA> convert_hangul_to_ipa(const Rune& hangul) const;
};
```

## 7. 성능 최적화

### 7.1 컴파일 타임 최적화
```cpp
// constexpr 유니코드 테이블
export namespace unicode::tables {
    constexpr std::array<unicode::Category, 0x110000> category_table =
        unicode::generate_category_table();

    constexpr std::array<language::Code, 0x110000> script_table =
        unicode::generate_script_table();

    template<char32_t CodePoint>
    constexpr unicode::Category get_category() {
        return category_table[CodePoint];
    }

    template<char32_t CodePoint>
    constexpr language::Code get_script() {
        return script_table[CodePoint];
    }
}

// SIMD 최적화된 문자열 처리
export namespace simd {
    // AVX2를 활용한 UTF-8 검증
    bool validate_utf8_avx2(std::string_view str) noexcept;

    // SIMD를 활용한 문자 분류
    std::vector<unicode::Category> classify_characters_simd(std::u32string_view str);
}
```

### 7.2 메모리 최적화
```cpp
// 문자열 인터닝 시스템
export class RuneStringInterner {
private:
    static inline std::unordered_set<RuneString> interned_strings_;
    static inline std::mutex mutex_;

public:
    static const RuneString& intern(RuneString str) {
        std::lock_guard lock(mutex_);
        auto [it, inserted] = interned_strings_.insert(std::move(str));
        return *it;
    }

    static void clear() {
        std::lock_guard lock(mutex_);
        interned_strings_.clear();
    }
};

// 지연 평가 기반 뷰
export template<RuneRange R>
class LazyRuneView {
private:
    R range_;
    mutable std::optional<RuneString> cached_result_;

public:
    explicit LazyRuneView(R range) : range_(std::move(range)) {}

    const RuneString& evaluate() const {
        if (!cached_result_) {
            cached_result_ = range_ | std::ranges::to<RuneString>();
        }
        return *cached_result_;
    }

    operator const RuneString&() const { return evaluate(); }
};
```

## 8. 빌드 시스템 (CMake + C++20 Modules)

### 8.1 CMake 설정
```cmake
cmake_minimum_required(VERSION 3.28)
project(rune_caster CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# C++20 Modules 지원
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

# 컴파일러별 설정
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "11.0")
        message(FATAL_ERROR "GCC 11+ required for C++20 modules")
    endif()
    target_compile_options(rune_caster PRIVATE -fcoroutines -fmodules-ts)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14.0")
        message(FATAL_ERROR "Clang 14+ required for C++20 modules")
    endif()
    target_compile_options(rune_caster PRIVATE -fmodules -fprebuilt-module-path=${CMAKE_BINARY_DIR})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.29")
        message(FATAL_ERROR "MSVC 19.29+ required for C++20 modules")
    endif()
    target_compile_options(rune_caster PRIVATE /experimental:module /std:c++20)
endif()

# 모듈 파일 정의
target_sources(rune_caster
    PUBLIC
        FILE_SET modules TYPE CXX_MODULES
        FILES
            src/rune.cppm
            src/spell.cppm
            src/caster.cppm
            src/phonetic.cppm
            src/language.cppm
            src/rune_caster.cppm
)
```

### 8.2 의존성 관리
```cmake
# ICU (Unicode 지원)
find_package(ICU REQUIRED COMPONENTS uc data)
target_link_libraries(rune_caster PRIVATE ICU::uc ICU::data)

# 벤치마킹 라이브러리
find_package(benchmark REQUIRED)
target_link_libraries(rune_caster_benchmark PRIVATE benchmark::benchmark)

# 테스트 프레임워크
find_package(GTest REQUIRED)
target_link_libraries(rune_caster_tests PRIVATE GTest::gtest GTest::gtest_main)
```

## 9. 예상 성능 특성

### 9.1 컴파일 시간
- **Modules 사용**: 기존 헤더 기반 대비 30-50% 단축
- **템플릿 인스턴스화**: Concepts 사용으로 오류 메시지 개선
- **병렬 컴파일**: 모듈 간 독립성으로 병렬 빌드 효율성 증대

### 9.2 런타임 성능
- **Zero-cost abstractions**: 인라인 최적화로 오버헤드 제거
- **SIMD 활용**: 문자열 처리에서 2-4배 성능 향상
- **메모리 지역성**: 캐시 친화적 데이터 구조 설계

### 9.3 메모리 사용량
- **문자열 인터닝**: 중복 문자열 제거로 메모리 절약
- **지연 평가**: 불필요한 계산 및 할당 방지
- **커스텀 할당자**: 메모리 풀을 통한 할당 오버헤드 감소

이 아키텍처는 C++20의 최신 기능들을 활용하여 타입 안전성, 성능, 가독성을 모두 만족하는 현대적인 라이브러리 설계를 제시합니다.