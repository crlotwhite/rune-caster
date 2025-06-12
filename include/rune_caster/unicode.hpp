#pragma once

#include <cstdint>

namespace rune_caster {
namespace unicode {

// 유니코드 카테고리
enum class Category : uint8_t {
    Unknown = 0,
    Letter,
    Mark,
    Number,
    Punctuation,
    Symbol,
    Separator,
    Other
};

// 유니코드 스크립트
enum class Script : uint16_t {
    Unknown = 0,
    Latin,
    Hangul,
    Hiragana,
    Katakana,
    Han,
    Arabic,
    Cyrillic,
    Greek,
    Hebrew
};

// 정규화 형식
enum class NormalizationForm {
    NFC,  // Canonical Decomposition, followed by Canonical Composition
    NFD,  // Canonical Decomposition
    NFKC, // Compatibility Decomposition, followed by Canonical Composition
    NFKD  // Compatibility Decomposition
};

// 유니코드 문자 속성 검사 함수들
constexpr bool is_ascii(char32_t cp) noexcept {
    return cp <= 0x7F;
}

constexpr bool is_latin(char32_t cp) noexcept {
    return (cp >= 0x0041 && cp <= 0x005A) ||  // A-Z
           (cp >= 0x0061 && cp <= 0x007A) ||  // a-z
           (cp >= 0x00C0 && cp <= 0x00FF);    // Latin-1 Supplement
}

constexpr bool is_hangul(char32_t cp) noexcept {
    return (cp >= 0x1100 && cp <= 0x11FF) ||  // Hangul Jamo
           (cp >= 0x3130 && cp <= 0x318F) ||  // Hangul Compatibility Jamo
           (cp >= 0xAC00 && cp <= 0xD7AF);    // Hangul Syllables
}

constexpr bool is_hiragana(char32_t cp) noexcept {
    return cp >= 0x3040 && cp <= 0x309F;
}

constexpr bool is_katakana(char32_t cp) noexcept {
    return cp >= 0x30A0 && cp <= 0x30FF;
}

constexpr bool is_kanji(char32_t cp) noexcept {
    return (cp >= 0x4E00 && cp <= 0x9FFF) ||  // CJK Unified Ideographs
           (cp >= 0x3400 && cp <= 0x4DBF);    // CJK Extension A
}

constexpr bool is_whitespace(char32_t cp) noexcept {
    return cp == U' ' || cp == U'\t' || cp == U'\n' || cp == U'\r' ||
           cp == U'\v' || cp == U'\f' || cp == 0x00A0;  // Non-breaking space
}

constexpr bool is_letter(char32_t cp) noexcept {
    return is_latin(cp) || is_hangul(cp) || is_hiragana(cp) ||
           is_katakana(cp) || is_kanji(cp);
}

constexpr bool is_digit(char32_t cp) noexcept {
    return cp >= U'0' && cp <= U'9';
}

constexpr bool is_punctuation(char32_t cp) noexcept {
    return (cp >= 0x0021 && cp <= 0x002F) ||  // ! " # $ % & ' ( ) * + , - . /
           (cp >= 0x003A && cp <= 0x0040) ||  // : ; < = > ? @
           (cp >= 0x005B && cp <= 0x0060) ||  // [ \ ] ^ _ `
           (cp >= 0x007B && cp <= 0x007E);    // { | } ~
}

// 카테고리 결정 함수
constexpr Category get_category(char32_t cp) noexcept {
    if (is_letter(cp)) return Category::Letter;
    if (is_digit(cp)) return Category::Number;
    if (is_punctuation(cp)) return Category::Punctuation;
    if (is_whitespace(cp)) return Category::Separator;
    return Category::Unknown;
}

// 스크립트 결정 함수
constexpr Script get_script(char32_t cp) noexcept {
    if (is_latin(cp)) return Script::Latin;
    if (is_hangul(cp)) return Script::Hangul;
    if (is_hiragana(cp)) return Script::Hiragana;
    if (is_katakana(cp)) return Script::Katakana;
    if (is_kanji(cp)) return Script::Han;
    return Script::Unknown;
}

} // namespace unicode
} // namespace rune_caster