# Rune Caster - API Design Document

## 1. 설계 원칙

### 1.1 STL 스타일 설계 철학
Rune Caster는 STL의 설계 철학을 계승하여 다음 원칙을 따릅니다:

- **직관성**: 기존 C++ 개발자에게 친숙한 인터페이스
- **일관성**: 모든 컴포넌트에서 일관된 명명 규칙과 패턴
- **조합성**: 작은 구성 요소들을 조합하여 복잡한 작업 수행
- **효율성**: Zero-cost abstractions과 최적화된 구현

### 1.2 C++20 기능 활용
```cpp
// Modules로 깔끔한 의존성 관리
import rune_caster;

// Concepts로 명확한 타입 제약
template<runes::RuneRange R>
auto process_text(R&& range);

// Ranges로 함수형 파이프라인
auto result = text
    | runes::views::clean_whitespace
    | runes::views::normalize
    | runes::views::detect_language;
```

## 2. 모듈 구조

### 2.1 핵심 모듈
```cpp
// 메인 모듈 - 모든 기능 재수출
export module rune_caster;

export import rune_caster.rune;      // 기본 타입 시스템
export import rune_caster.spell;     // 알고리즘 컬렉션
export import rune_caster.caster;    // 실행 엔진
export import rune_caster.phonetic;  // 음성학적 처리
export import rune_caster.language;  // 언어별 특화 기능
export import rune_caster.views;     // Range 뷰 어댑터
```

### 2.2 선택적 확장 모듈
```cpp
// 고급 기능 모듈들
import rune_caster.ml;          // 기계학습 연동
import rune_caster.gpu;         // GPU 가속 처리
import rune_caster.async;       // 비동기 처리
import rune_caster.bindings;    // 언어 바인딩 (Python, JS)
```

## 3. 기본 타입 시스템

### 3.1 Rune 타입
```cpp
export module rune_caster.rune;

// 단일 유니코드 문자
export class Rune {
public:
    // 생성자
    constexpr Rune() noexcept = default;
    constexpr Rune(char32_t codepoint) noexcept;
    constexpr Rune(char32_t codepoint, language::Code hint) noexcept;

    // 정적 팩토리 메서드
    static constexpr Rune from_utf8(std::string_view utf8);
    static constexpr Rune from_utf16(std::u16string_view utf16);

    // 속성 접근
    constexpr char32_t code_point() const noexcept;
    constexpr unicode::Category category() const noexcept;
    constexpr unicode::Script script() const noexcept;
    constexpr language::Code language_hint() const noexcept;

    // 언어별 속성 (constexpr 지원)
    constexpr bool is_ascii() const noexcept;
    constexpr bool is_latin() const noexcept;
    constexpr bool is_hangul() const noexcept;
    constexpr bool is_hiragana() const noexcept;
    constexpr bool is_katakana() const noexcept;
    constexpr bool is_kanji() const noexcept;
    constexpr bool is_emoji() const noexcept;

    // 일반 속성
    constexpr bool is_letter() const noexcept;
    constexpr bool is_digit() const noexcept;
    constexpr bool is_whitespace() const noexcept;
    constexpr bool is_punctuation() const noexcept;
    constexpr bool is_symbol() const noexcept;

    // 변환
    std::string to_utf8() const;
    std::u16string to_utf16() const;
    std::u32string to_utf32() const;

    // 음성학적 변환
    phonetic::IPA to_ipa() const;
    std::vector<phonetic::IPA> to_ipa_variants() const;

    // 비교 연산자 (C++20 삼중 비교)
    auto operator<=>(const Rune& other) const noexcept = default;
    bool operator==(const Rune& other) const noexcept = default;
};

// 사용자 정의 리터럴
export constexpr Rune operator""_rune(char32_t cp) noexcept {
    return Rune(cp);
}
```

### 3.2 RuneString 타입
```cpp
// 동적 길이 유니코드 문자열
export class RuneString {
public:
    // STL 호환 타입 정의
    using value_type = Rune;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = std::vector<Rune>::iterator;
    using const_iterator = std::vector<Rune>::const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // 생성자
    RuneString() = default;
    RuneString(const RuneString&) = default;
    RuneString(RuneString&&) = default;
    RuneString& operator=(const RuneString&) = default;
    RuneString& operator=(RuneString&&) = default;

    explicit RuneString(std::string_view utf8);
    explicit RuneString(std::u16string_view utf16);
    explicit RuneString(std::u32string_view utf32);
    explicit RuneString(std::initializer_list<Rune> runes);

    template<std::input_iterator It>
    requires std::convertible_to<std::iter_value_t<It>, Rune>
    RuneString(It first, It last);

    template<std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, Rune>
    explicit RuneString(R&& range);

    // 이터레이터
    iterator begin() noexcept { return runes_.begin(); }
    iterator end() noexcept { return runes_.end(); }
    const_iterator begin() const noexcept { return runes_.begin(); }
    const_iterator end() const noexcept { return runes_.end(); }
    const_iterator cbegin() const noexcept { return runes_.cbegin(); }
    const_iterator cend() const noexcept { return runes_.cend(); }

    reverse_iterator rbegin() noexcept { return runes_.rbegin(); }
    reverse_iterator rend() noexcept { return runes_.rend(); }
    const_reverse_iterator rbegin() const noexcept { return runes_.rbegin(); }
    const_reverse_iterator rend() const noexcept { return runes_.rend(); }
    const_reverse_iterator crbegin() const noexcept { return runes_.crbegin(); }
    const_reverse_iterator crend() const noexcept { return runes_.crend(); }

    // 크기 및 용량
    size_type size() const noexcept { return runes_.size(); }
    size_type length() const noexcept { return size(); }
    bool empty() const noexcept { return runes_.empty(); }
    size_type capacity() const noexcept { return runes_.capacity(); }
    size_type max_size() const noexcept { return runes_.max_size(); }

    void reserve(size_type new_cap) { runes_.reserve(new_cap); }
    void shrink_to_fit() { runes_.shrink_to_fit(); }

    // 원소 접근
    Rune& operator[](size_type pos) { return runes_[pos]; }
    const Rune& operator[](size_type pos) const { return runes_[pos]; }
    Rune& at(size_type pos) { return runes_.at(pos); }
    const Rune& at(size_type pos) const { return runes_.at(pos); }
    Rune& front() { return runes_.front(); }
    const Rune& front() const { return runes_.front(); }
    Rune& back() { return runes_.back(); }
    const Rune& back() const { return runes_.back(); }

    // 수정자
    void clear() noexcept { runes_.clear(); }
    void push_back(const Rune& rune) { runes_.push_back(rune); }
    void push_back(Rune&& rune) { runes_.push_back(std::move(rune)); }
    void pop_back() { runes_.pop_back(); }

    template<typename... Args>
    Rune& emplace_back(Args&&... args) {
        return runes_.emplace_back(std::forward<Args>(args)...);
    }

    // 문자열 연산
    RuneString& append(const RuneString& other);
    RuneString& append(const Rune& rune);
    RuneString& append(std::string_view utf8);

    RuneString substr(size_type pos = 0, size_type len = npos) const;

    // 검색
    size_type find(const Rune& rune, size_type pos = 0) const noexcept;
    size_type find(const RuneString& str, size_type pos = 0) const noexcept;
    size_type rfind(const Rune& rune, size_type pos = npos) const noexcept;

    // 변환
    std::string to_utf8() const;
    std::u16string to_utf16() const;
    std::u32string to_utf32() const;

    // 언어 분석
    language::Code detect_primary_language() const;
    std::vector<language::DetectionResult> detect_all_languages() const;

    // Ranges 지원
    auto as_range() const { return std::views::all(runes_); }

    // 비교 연산자
    auto operator<=>(const RuneString& other) const noexcept;
    bool operator==(const RuneString& other) const noexcept;

    static constexpr size_type npos = std::numeric_limits<size_type>::max();

private:
    std::vector<Rune> runes_;
};

// 사용자 정의 리터럴
export RuneString operator""_rs(const char* str, std::size_t len) {
    return RuneString(std::string_view(str, len));
}
```

### 3.3 RuneView 타입
```cpp
// 경량 읽기 전용 뷰 (std::string_view와 유사)
export class RuneView {
public:
    using value_type = Rune;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = const Rune*;
    using const_iterator = const Rune*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // 생성자
    constexpr RuneView() noexcept = default;
    constexpr RuneView(const Rune* data, size_type size) noexcept;
    constexpr RuneView(const RuneString& str) noexcept;

    template<std::contiguous_iterator It>
    requires std::same_as<std::iter_value_t<It>, Rune>
    constexpr RuneView(It first, It last) noexcept;

    template<std::ranges::contiguous_range R>
    requires std::same_as<std::ranges::range_value_t<R>, Rune>
    constexpr RuneView(R&& range) noexcept;

    // 이터레이터
    constexpr const_iterator begin() const noexcept { return data_; }
    constexpr const_iterator end() const noexcept { return data_ + size_; }
    constexpr const_iterator cbegin() const noexcept { return begin(); }
    constexpr const_iterator cend() const noexcept { return end(); }

    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

    // 크기
    constexpr size_type size() const noexcept { return size_; }
    constexpr size_type length() const noexcept { return size(); }
    constexpr bool empty() const noexcept { return size_ == 0; }

    // 원소 접근
    constexpr const Rune& operator[](size_type pos) const noexcept;
    constexpr const Rune& at(size_type pos) const;
    constexpr const Rune& front() const noexcept;
    constexpr const Rune& back() const noexcept;
    constexpr const Rune* data() const noexcept { return data_; }

    // 부분 문자열
    constexpr RuneView substr(size_type pos = 0, size_type len = npos) const;
    constexpr RuneView prefix(size_type len) const;
    constexpr RuneView suffix(size_type len) const;

    // 검색
    constexpr size_type find(const Rune& rune, size_type pos = 0) const noexcept;
    constexpr size_type find(RuneView str, size_type pos = 0) const noexcept;
    constexpr size_type rfind(const Rune& rune, size_type pos = npos) const noexcept;

    // 비교
    constexpr bool starts_with(RuneView prefix) const noexcept;
    constexpr bool ends_with(RuneView suffix) const noexcept;
    constexpr bool contains(RuneView substr) const noexcept;
    constexpr bool contains(const Rune& rune) const noexcept;

    // 변환 (새로운 객체 생성)
    RuneString to_string() const;
    std::string to_utf8() const;
    std::u16string to_utf16() const;
    std::u32string to_utf32() const;

    // 비교 연산자
    constexpr auto operator<=>(RuneView other) const noexcept;
    constexpr bool operator==(RuneView other) const noexcept;

    static constexpr size_type npos = std::numeric_limits<size_type>::max();

private:
    const Rune* data_ = nullptr;
    size_type size_ = 0;
};
```

## 4. Concepts 정의

### 4.1 핵심 Concepts
```cpp
export module rune_caster.concepts;

import std;

namespace runes {

// 기본 Rune 타입 Concepts
template<typename T>
concept RuneLike = requires(T t) {
    { t.code_point() } -> std::convertible_to<char32_t>;
    { t.category() } -> std::convertible_to<unicode::Category>;
    { t.script() } -> std::convertible_to<unicode::Script>;
};

template<typename T>
concept RuneContainer = requires(T t) {
    typename T::value_type;
    typename T::iterator;
    typename T::const_iterator;
    requires RuneLike<typename T::value_type>;
    { t.begin() } -> std::input_iterator;
    { t.end() } -> std::input_iterator;
    { t.size() } -> std::convertible_to<std::size_t>;
    { t.empty() } -> std::convertible_to<bool>;
};

template<typename T>
concept RuneRange = std::ranges::range<T> &&
                   RuneLike<std::ranges::range_value_t<T>>;

template<typename T>
concept MutableRuneRange = RuneRange<T> &&
                          std::ranges::output_range<T, Rune>;

// 텍스트 처리 Concepts
template<typename T>
concept TextProcessor = requires(T processor, RuneView input) {
    { processor.process(input) } -> RuneRange;
};

template<typename T>
concept Spell = TextProcessor<T> && requires(T spell) {
    typename T::input_type;
    typename T::output_type;
    requires RuneRange<typename T::input_type>;
    requires RuneRange<typename T::output_type>;
};

template<typename T>
concept InPlaceSpell = Spell<T> && requires {
    requires std::same_as<typename T::input_type, typename T::output_type>;
};

template<typename T>
concept StreamingSpell = Spell<T> && requires(T spell) {
    { spell.process_stream(std::declval<std::ranges::input_range auto>()) }
        -> std::ranges::range;
};

// 언어별 Concepts
template<typename T, language::Code Lang>
concept LanguageSpecificProcessor = TextProcessor<T> && requires {
    { T::supported_language } -> std::convertible_to<language::Code>;
    requires T::supported_language == Lang;
};

template<typename T>
concept LanguageDetector = requires(T detector, RuneView text) {
    { detector.detect(text) } -> std::convertible_to<language::Code>;
    { detector.detect_all(text) } -> std::ranges::range;
};

// 음성학적 처리 Concepts
template<typename T>
concept PhoneticConverter = requires(T converter, RuneView text) {
    { converter.to_phonetic(text) } -> RuneRange;
    { converter.from_phonetic(std::declval<RuneRange auto>()) } -> RuneRange;
};

template<typename T>
concept IPAProvider = requires(T provider, Rune rune) {
    { provider.to_ipa(rune) } -> std::convertible_to<phonetic::IPA>;
    { provider.to_ipa_variants(rune) } -> std::ranges::range;
};

}
```

### 4.2 제약 사항 Concepts
```cpp
namespace runes::constraints {

// 성능 제약
template<typename T>
concept LowLatency = requires {
    requires sizeof(T) <= 64; // 캐시 라인 크기 제한
};

template<typename T>
concept Copyable = std::copyable<T>;

template<typename T>
concept Movable = std::movable<T>;

template<typename T>
concept ThreadSafe = requires {
    typename T::is_thread_safe;
    requires T::is_thread_safe::value;
};

// 메모리 제약
template<typename T>
concept NoAlloc = requires {
    typename T::allocates_memory;
    requires !T::allocates_memory::value;
};

template<typename T>
concept Stackable = NoAlloc<T> && LowLatency<T>;

}
```

## 5. Spell 알고리즘 API

### 5.1 기본 Spell들
```cpp
export module rune_caster.spell;

import rune_caster.concepts;
import rune_caster.rune;

namespace spells {

// 공백 정제
export class CleanWhitespace {
public:
    using input_type = RuneView;
    using output_type = RuneString;

    struct Options {
        bool collapse_whitespace = true;
        bool trim_leading = true;
        bool trim_trailing = true;
        bool normalize_newlines = true;
    };

    explicit CleanWhitespace(Options opts = {}) : options_(opts) {}

    RuneString process(RuneView input) const;

    // Ranges 지원
    template<runes::RuneRange R>
    auto operator()(R&& range) const {
        return process(RuneView(range));
    }

private:
    Options options_;
};

// 유니코드 정규화
export template<unicode::NormalizationForm Form = unicode::NormalizationForm::NFC>
class NormalizeUnicode {
public:
    using input_type = RuneView;
    using output_type = RuneString;

    constexpr NormalizeUnicode() = default;

    RuneString process(RuneView input) const;

    template<runes::RuneRange R>
    auto operator()(R&& range) const {
        return process(RuneView(range));
    }
};

// 대소문자 변환
export class ChangeCase {
public:
    enum class Mode { Lower, Upper, Title, Fold };

    using input_type = RuneView;
    using output_type = RuneString;

    explicit ChangeCase(Mode mode, language::Code locale = language::Code::Unknown)
        : mode_(mode), locale_(locale) {}

    RuneString process(RuneView input) const;

    // 정적 편의 함수들
    static ChangeCase to_lower(language::Code locale = language::Code::Unknown) {
        return ChangeCase(Mode::Lower, locale);
    }

    static ChangeCase to_upper(language::Code locale = language::Code::Unknown) {
        return ChangeCase(Mode::Upper, locale);
    }

    static ChangeCase to_title(language::Code locale = language::Code::Unknown) {
        return ChangeCase(Mode::Title, locale);
    }

    template<runes::RuneRange R>
    auto operator()(R&& range) const {
        return process(RuneView(range));
    }

private:
    Mode mode_;
    language::Code locale_;
};

// 언어 감지
export class DetectLanguage {
public:
    using input_type = RuneView;
    using output_type = language::DetectionResult;

    struct Options {
        std::size_t min_text_length = 10;
        double confidence_threshold = 0.7;
        bool detect_mixed_languages = false;
    };

    explicit DetectLanguage(Options opts = {}) : options_(opts) {}

    language::DetectionResult process(RuneView input) const;

    // 다중 언어 감지
    std::vector<language::DetectionResult> detect_all(RuneView input) const;

    template<runes::RuneRange R>
    auto operator()(R&& range) const {
        return process(RuneView(range));
    }

private:
    Options options_;
};

// 필터링
export template<typename Predicate>
class Filter {
public:
    using input_type = RuneView;
    using output_type = RuneString;

    explicit Filter(Predicate pred) : predicate_(std::move(pred)) {}

    RuneString process(RuneView input) const {
        return input
            | std::views::filter(predicate_)
            | std::ranges::to<RuneString>();
    }

    template<runes::RuneRange R>
    auto operator()(R&& range) const {
        return process(RuneView(range));
    }

private:
    Predicate predicate_;
};

// 팩토리 함수들
export auto clean_whitespace(CleanWhitespace::Options opts = {}) {
    return CleanWhitespace(opts);
}

export auto normalize_unicode() {
    return NormalizeUnicode<unicode::NormalizationForm::NFC>();
}

export template<unicode::NormalizationForm Form>
auto normalize_unicode() {
    return NormalizeUnicode<Form>();
}

export auto to_lower(language::Code locale = language::Code::Unknown) {
    return ChangeCase::to_lower(locale);
}

export auto to_upper(language::Code locale = language::Code::Unknown) {
    return ChangeCase::to_upper(locale);
}

export auto detect_language(DetectLanguage::Options opts = {}) {
    return DetectLanguage(opts);
}

export template<typename Predicate>
auto filter(Predicate pred) {
    return Filter(std::move(pred));
}

// 편의 필터들
export auto remove_punctuation() {
    return filter([](const Rune& r) { return !r.is_punctuation(); });
}

export auto keep_letters_only() {
    return filter([](const Rune& r) { return r.is_letter(); });
}

export auto remove_whitespace() {
    return filter([](const Rune& r) { return !r.is_whitespace(); });
}

}
```

### 5.2 Spell 조합 API
```cpp
namespace spells {

// Spell 파이프라인
export template<runes::Spell... Spells>
class Pipeline {
public:
    explicit Pipeline(Spells... spells) : spells_(std::move(spells)...) {}

    template<runes::RuneRange Input>
    auto process(Input&& input) const {
        return std::apply([&input](const auto&... spells) {
            return (input | ... | spells);
        }, spells_);
    }

    template<runes::RuneRange R>
    auto operator()(R&& range) const {
        return process(std::forward<R>(range));
    }

    // 새로운 Spell 추가
    template<runes::Spell NewSpell>
    auto then(NewSpell spell) const {
        return std::apply([spell](const auto&... existing) {
            return Pipeline(existing..., spell);
        }, spells_);
    }

private:
    std::tuple<Spells...> spells_;
};

// 조건부 Spell
export template<typename Predicate, runes::Spell TrueSpell, runes::Spell FalseSpell>
class Conditional {
public:
    Conditional(Predicate pred, TrueSpell true_spell, FalseSpell false_spell)
        : predicate_(std::move(pred))
        , true_spell_(std::move(true_spell))
        , false_spell_(std::move(false_spell)) {}

    template<runes::RuneRange Input>
    auto process(Input&& input) const {
        if (predicate_(input)) {
            return true_spell_(std::forward<Input>(input));
        } else {
            return false_spell_(std::forward<Input>(input));
        }
    }

    template<runes::RuneRange R>
    auto operator()(R&& range) const {
        return process(std::forward<R>(range));
    }

private:
    Predicate predicate_;
    TrueSpell true_spell_;
    FalseSpell false_spell_;
};

// 팩토리 함수들
export template<runes::Spell... Spells>
auto make_pipeline(Spells... spells) {
    return Pipeline(std::move(spells)...);
}

export template<typename Predicate, runes::Spell TrueSpell, runes::Spell FalseSpell>
auto make_conditional(Predicate pred, TrueSpell true_spell, FalseSpell false_spell) {
    return Conditional(std::move(pred), std::move(true_spell), std::move(false_spell));
}

// 자주 사용되는 파이프라인들
export auto standard_cleanup() {
    return make_pipeline(
        clean_whitespace(),
        normalize_unicode(),
        to_lower()
    );
}

export auto aggressive_cleanup() {
    return make_pipeline(
        clean_whitespace(),
        normalize_unicode(),
        remove_punctuation(),
        to_lower()
    );
}

}
```

## 6. Caster 실행 API

### 6.1 기본 Caster
```cpp
export module rune_caster.caster;

import rune_caster.concepts;
import rune_caster.spell;

namespace casters {

// 즉시 실행 Caster
export template<runes::Spell SpellType>
class Immediate {
public:
    explicit Immediate(SpellType spell) : spell_(std::move(spell)) {}

    template<runes::RuneRange Input>
    auto cast(Input&& input) const {
        return spell_(std::forward<Input>(input));
    }

    // 파이프라인 연산자
    template<runes::Spell OtherSpell>
    auto operator|(OtherSpell other) const {
        return Immediate(spells::make_pipeline(spell_, std::move(other)));
    }

private:
    SpellType spell_;
};

// 지연 실행 Caster
export template<runes::Spell SpellType>
class Lazy {
public:
    explicit Lazy(SpellType spell) : spell_(std::move(spell)) {}

    template<runes::RuneRange Input>
    auto cast(Input&& input) const {
        return std::views::transform(input, [this](const auto& elem) {
            return spell_(RuneView(&elem, 1));
        }) | std::views::join;
    }

private:
    SpellType spell_;
};

// 스트림 Caster
export template<runes::Spell SpellType>
class Stream {
public:
    explicit Stream(SpellType spell, std::size_t chunk_size = 4096)
        : spell_(std::move(spell)), chunk_size_(chunk_size) {}

    template<std::ranges::input_range Input>
    requires runes::RuneLike<std::ranges::range_value_t<Input>>
    auto cast(Input&& input) const {
        return input
            | std::views::chunk(chunk_size_)
            | std::views::transform([this](auto chunk) {
                return spell_(chunk | std::ranges::to<RuneString>());
            });
    }

private:
    SpellType spell_;
    std::size_t chunk_size_;
};

// 병렬 Caster
export template<runes::Spell SpellType>
class Parallel {
public:
    explicit Parallel(SpellType spell, std::size_t thread_count = 0)
        : spell_(std::move(spell))
        , thread_count_(thread_count == 0 ? std::thread::hardware_concurrency() : thread_count) {}

    template<runes::RuneRange Input>
    auto cast(Input&& input) const {
        if (std::ranges::size(input) < 1000) {
            // 작은 입력은 순차 처리
            return spell_(std::forward<Input>(input));
        }

        return cast_parallel(std::forward<Input>(input));
    }

private:
    SpellType spell_;
    std::size_t thread_count_;

    template<runes::RuneRange Input>
    auto cast_parallel(Input&& input) const;
};

// 팩토리 함수들
export template<runes::Spell SpellType>
auto immediate(SpellType spell) {
    return Immediate(std::move(spell));
}

export template<runes::Spell SpellType>
auto lazy(SpellType spell) {
    return Lazy(std::move(spell));
}

export template<runes::Spell SpellType>
auto stream(SpellType spell, std::size_t chunk_size = 4096) {
    return Stream(std::move(spell), chunk_size);
}

export template<runes::Spell SpellType>
auto parallel(SpellType spell, std::size_t thread_count = 0) {
    return Parallel(std::move(spell), thread_count);
}

}
```

## 7. Range Views API

### 7.1 커스텀 Views
```cpp
export module rune_caster.views;

import std;
import rune_caster.rune;
import rune_caster.spell;

namespace runes::views {

// 텍스트 정제 뷰
export inline constexpr auto clean_whitespace = []<std::ranges::range R>(R&& range) {
    return range | std::views::filter([](const Rune& r) {
        return !r.is_whitespace() || r.code_point() == U' ';
    }) | std::views::transform([](const Rune& r) {
        return r.is_whitespace() ? Rune(U' ') : r;
    });
};

// 정규화 뷰
export inline constexpr auto normalize = []<std::ranges::range R>(R&& range) {
    return range | std::views::transform([](const Rune& r) {
        // 간단한 정규화 로직
        return r; // 실제로는 유니코드 정규화 수행
    });
};

// 언어별 필터
export template<language::Code Lang>
inline constexpr auto filter_language = []<std::ranges::range R>(R&& range) {
    return range | std::views::filter([](const Rune& r) {
        return r.script() == language::to_script(Lang);
    });
};

// 문자 타입별 필터
export inline constexpr auto letters_only = []<std::ranges::range R>(R&& range) {
    return range | std::views::filter([](const Rune& r) {
        return r.is_letter();
    });
};

export inline constexpr auto digits_only = []<std::ranges::range R>(R&& range) {
    return range | std::views::filter([](const Rune& r) {
        return r.is_digit();
    });
};

export inline constexpr auto no_punctuation = []<std::ranges::range R>(R&& range) {
    return range | std::views::filter([](const Rune& r) {
        return !r.is_punctuation();
    });
};

// 대소문자 변환 뷰
export inline constexpr auto to_lower = []<std::ranges::range R>(R&& range) {
    return range | std::views::transform([](const Rune& r) {
        return unicode::to_lower(r);
    });
};

export inline constexpr auto to_upper = []<std::ranges::range R>(R&& range) {
    return range | std::views::transform([](const Rune& r) {
        return unicode::to_upper(r);
    });
};

// 청킹 뷰 (언어 단위)
export inline constexpr auto chunk_by_language = []<std::ranges::range R>(R&& range) {
    return range | std::views::chunk_by([](const Rune& a, const Rune& b) {
        return a.script() == b.script();
    });
};

// 음성학적 변환 뷰
export inline constexpr auto to_phonetic = []<std::ranges::range R>(R&& range) {
    return range | std::views::transform([](const Rune& r) {
        return r.to_ipa();
    });
};

}
```

## 8. 사용 예제

### 8.1 기본 사용법
```cpp
import rune_caster;

using namespace runes;

int main() {
    // 1. 기본 문자열 생성
    auto text = "Hello 안녕하세요 こんにちは!"_rs;

    // 2. Range-based 처리
    auto cleaned = text
        | views::clean_whitespace
        | views::normalize
        | views::to_lower;

    // 3. Spell 사용
    auto result = spells::standard_cleanup()(text);

    // 4. Caster 사용
    auto caster = casters::immediate(spells::aggressive_cleanup());
    auto processed = caster.cast(text);

    // 5. 파이프라인 구성
    auto pipeline = spells::make_pipeline(
        spells::clean_whitespace(),
        spells::normalize_unicode(),
        spells::detect_language()
    );

    // 6. 언어별 처리
    auto korean_text = text | views::filter_language<language::Code::Korean>;

    return 0;
}
```

### 8.2 고급 사용법
```cpp
import rune_caster;
import rune_caster.phonetic;

using namespace runes;

void advanced_processing() {
    // 1. 조건부 처리
    auto conditional_processor = spells::make_conditional(
        [](const auto& input) { return std::ranges::size(input) > 100; },
        spells::aggressive_cleanup(),
        spells::standard_cleanup()
    );

    // 2. 스트리밍 처리
    std::ifstream file("large_text.txt");
    auto stream_caster = casters::stream(spells::standard_cleanup(), 8192);

    // 3. 병렬 처리
    auto parallel_caster = casters::parallel(spells::aggressive_cleanup(), 4);

    // 4. 언어 감지 및 특화 처리
    auto text = "한글과 English와 日本語가 섞인 텍스트"_rs;
    auto lang_chunks = text | views::chunk_by_language;

    for (const auto& chunk : lang_chunks) {
        auto detected = spells::detect_language()(chunk);
        // 언어별 특화 처리...
    }

    // 5. 음성학적 변환
    auto phonetic_result = text
        | views::letters_only
        | views::to_phonetic;

    // 6. 사용자 정의 Spell
    auto custom_spell = spells::filter([](const Rune& r) {
        return r.code_point() < 0x10000; // BMP 문자만 유지
    });

    auto result = text | custom_spell;
}
```

### 8.3 타입 안전성 예제
```cpp
// Concepts를 활용한 제네릭 함수
template<runes::RuneRange R>
auto safe_process(R&& range) {
    static_assert(runes::RuneRange<R>, "Input must be a valid rune range");

    return range
        | views::clean_whitespace
        | views::normalize;
}

// 컴파일 타임 검증
template<runes::Spell S>
void validate_spell(S&& spell) {
    static_assert(runes::Spell<S>, "Must be a valid spell");

    // 타입이 보장된 안전한 처리
}
```

## 9. 오류 처리 및 예외

### 9.1 예외 계층
```cpp
export module rune_caster.exceptions;

import std;

namespace runes {

// 기본 예외 클래스
export class RuneCasterError : public std::exception {
public:
    explicit RuneCasterError(std::string message) : message_(std::move(message)) {}
    const char* what() const noexcept override { return message_.c_str(); }

private:
    std::string message_;
};

// 유니코드 관련 오류
export class UnicodeError : public RuneCasterError {
public:
    explicit UnicodeError(std::string message) : RuneCasterError(std::move(message)) {}
};

// 언어 처리 오류
export class LanguageError : public RuneCasterError {
public:
    explicit LanguageError(std::string message) : RuneCasterError(std::move(message)) {}
};

// 음성학적 변환 오류
export class PhoneticError : public RuneCasterError {
public:
    explicit PhoneticError(std::string message) : RuneCasterError(std::move(message)) {}
};

}
```

### 9.2 오류 코드 시스템
```cpp
export enum class ErrorCode {
    Success = 0,
    InvalidUnicode,
    UnsupportedLanguage,
    InsufficientData,
    ConversionFailed,
    ResourceExhausted
};

export class Result {
public:
    template<typename T>
    static Result ok(T&& value) {
        return Result(ErrorCode::Success, std::forward<T>(value));
    }

    static Result error(ErrorCode code, std::string message = "") {
        return Result(code, std::move(message));
    }

    bool is_ok() const noexcept { return code_ == ErrorCode::Success; }
    bool is_error() const noexcept { return !is_ok(); }

    ErrorCode error_code() const noexcept { return code_; }
    const std::string& error_message() const noexcept { return message_; }

private:
    Result(ErrorCode code, std::string message)
        : code_(code), message_(std::move(message)) {}

    ErrorCode code_;
    std::string message_;
};
```

이 API 설계는 C++20의 강력한 기능들을 활용하여 타입 안전하고 성능이 뛰어나며 사용하기 쉬운 인터페이스를 제공합니다. Concepts를 통한 컴파일 타임 검증, Ranges를 통한 함수형 프로그래밍 패러다임, 그리고 Modules를 통한 깔끔한 의존성 관리가 핵심 특징입니다.