#pragma once

#include <concepts>
#include <ranges>
#include <type_traits>

namespace rune_caster {

// Forward declarations
namespace unicode {
    enum class Category;
    enum class Script;
}

namespace language {
    enum class Code;
}

namespace runes {

// 기본 Rune 타입 Concepts
template<typename T>
concept RuneLike = requires(T t) {
    { t.code_point() } -> std::convertible_to<char32_t>;
    { t.category() } -> std::convertible_to<unicode::Category>;
    { t.script() } -> std::convertible_to<unicode::Script>;
    { t.language_hint() } -> std::convertible_to<language::Code>;
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
                          std::ranges::output_range<T, typename std::ranges::range_value_t<T>>;

// 텍스트 처리 Concepts
template<typename T>
concept TextProcessor = requires(T processor) {
    typename T::input_type;
    typename T::output_type;
    requires RuneRange<typename T::input_type>;
    requires RuneRange<typename T::output_type>;
} && requires(T processor, typename T::input_type input) {
    { processor.process(input) } -> std::convertible_to<typename T::output_type>;
};

template<typename T>
concept Spell = TextProcessor<T> && requires(T spell) {
    // Spell은 함수 호출 연산자도 지원해야 함
    { spell(std::declval<typename T::input_type>()) }
        -> std::convertible_to<typename T::output_type>;
};

template<typename T>
concept InPlaceSpell = Spell<T> && requires {
    requires std::same_as<typename T::input_type, typename T::output_type>;
};

// Caster Concepts
template<typename T>
concept Caster = requires(T caster) {
    typename T::input_type;
    typename T::output_type;
    { caster.cast(std::declval<typename T::input_type>()) }
        -> std::convertible_to<typename T::output_type>;
};

// 성능 제약 Concepts
template<typename T>
concept LowLatency = requires {
    requires sizeof(T) <= 64; // 캐시 라인 크기 제한
};

template<typename T>
concept ThreadSafe = requires {
    typename T::is_thread_safe;
    requires T::is_thread_safe::value;
};

} // namespace runes
} // namespace rune_caster
