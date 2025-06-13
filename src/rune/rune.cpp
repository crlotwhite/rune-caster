#include "rune_caster/rune.hpp"
#include <stdexcept>
#include <codecvt>
#include <locale>

namespace rune_caster {

// === Constructors (C++20 constexpr enhanced) ===

constexpr Rune::Rune() noexcept
    : codepoint_(U'\0')
    , language_(language::Code::Unknown)
    , phoneme_()
{
}

constexpr Rune::Rune(char32_t codepoint) noexcept
    : codepoint_(codepoint)
    , language_(detect_language(codepoint))
    , phoneme_()
{
}

constexpr Rune::Rune(char32_t codepoint, language::Code lang) noexcept
    : codepoint_(codepoint)
    , language_(lang)
    , phoneme_()
{
}

Rune::Rune(char32_t codepoint, language::Code lang, std::string phoneme)
    : codepoint_(codepoint)
    , language_(lang)
    , phoneme_(std::move(phoneme))
{
}

// === Category and Script accessors (constexpr) ===

constexpr unicode::Category Rune::category() const noexcept {
    return unicode::get_category(codepoint_);
}

constexpr unicode::Script Rune::script() const noexcept {
    return unicode::get_script(codepoint_);
}

// === Enhanced Character classification (C++20 constexpr) ===

constexpr bool Rune::is_vowel() const noexcept {
    // Basic Latin vowels
    switch (codepoint_) {
        case U'a': case U'e': case U'i': case U'o': case U'u':
        case U'A': case U'E': case U'I': case U'O': case U'U':
            return true;

        // Korean vowels (simplified)
        case U'ㅏ': case U'ㅑ': case U'ㅓ': case U'ㅕ': case U'ㅗ':
        case U'ㅛ': case U'ㅜ': case U'ㅠ': case U'ㅡ': case U'ㅣ':
        case U'ㅐ': case U'ㅒ': case U'ㅔ': case U'ㅖ': case U'ㅘ':
        case U'ㅙ': case U'ㅚ': case U'ㅝ': case U'ㅞ': case U'ㅟ':
        case U'ㅢ':
            return true;

        // Japanese vowels (Hiragana)
        case U'あ': case U'い': case U'う': case U'え': case U'お':
        case U'ア': case U'イ': case U'ウ': case U'エ': case U'オ':
            return true;

        default:
            return false;
    }
}

constexpr bool Rune::is_consonant() const noexcept {
    return is_letter() && !is_vowel();
}

constexpr bool Rune::is_letter() const noexcept {
    return unicode::is_letter(codepoint_);
}

constexpr bool Rune::is_digit() const noexcept {
    return unicode::is_digit(codepoint_);
}

constexpr bool Rune::is_whitespace() const noexcept {
    return unicode::is_whitespace(codepoint_);
}

constexpr bool Rune::is_punctuation() const noexcept {
    return unicode::is_punctuation(codepoint_);
}

// === Language-specific properties (API Design Document requirement) ===

constexpr bool Rune::is_ascii() const noexcept {
    return codepoint_ <= 0x7F;
}

constexpr bool Rune::is_latin() const noexcept {
    // Basic Latin (U+0000-U+007F) and Latin-1 Supplement (U+0080-U+00FF)
    // Latin Extended-A (U+0100-U+017F) and Latin Extended-B (U+0180-U+024F)
    // Latin Extended Additional (U+1E00-U+1EFF)
    return (codepoint_ >= 0x0000 && codepoint_ <= 0x024F) ||
           (codepoint_ >= 0x1E00 && codepoint_ <= 0x1EFF);
}

constexpr bool Rune::is_hangul() const noexcept {
    // Hangul Jamo (U+1100-U+11FF)
    // Hangul Compatibility Jamo (U+3130-U+318F)
    // Hangul Syllables (U+AC00-U+D7AF)
    return (codepoint_ >= 0x1100 && codepoint_ <= 0x11FF) ||
           (codepoint_ >= 0x3130 && codepoint_ <= 0x318F) ||
           (codepoint_ >= 0xAC00 && codepoint_ <= 0xD7AF);
}

constexpr bool Rune::is_hiragana() const noexcept {
    // Hiragana (U+3040-U+309F)
    return codepoint_ >= 0x3040 && codepoint_ <= 0x309F;
}

constexpr bool Rune::is_katakana() const noexcept {
    // Katakana (U+30A0-U+30FF)
    // Katakana Phonetic Extensions (U+31F0-U+31FF)
    return (codepoint_ >= 0x30A0 && codepoint_ <= 0x30FF) ||
           (codepoint_ >= 0x31F0 && codepoint_ <= 0x31FF);
}

constexpr bool Rune::is_kanji() const noexcept {
    // CJK Unified Ideographs (U+4E00-U+9FFF)
    // CJK Unified Ideographs Extension A (U+3400-U+4DBF)
    // CJK Unified Ideographs Extension B (U+20000-U+2A6DF)
    return (codepoint_ >= 0x4E00 && codepoint_ <= 0x9FFF) ||
           (codepoint_ >= 0x3400 && codepoint_ <= 0x4DBF) ||
           (codepoint_ >= 0x20000 && codepoint_ <= 0x2A6DF);
}

constexpr bool Rune::is_emoji() const noexcept {
    // Emoticons (U+1F600-U+1F64F)
    // Miscellaneous Symbols and Pictographs (U+1F300-U+1F5FF)
    // Transport and Map Symbols (U+1F680-U+1F6FF)
    // Supplemental Symbols and Pictographs (U+1F900-U+1F9FF)
    return (codepoint_ >= 0x1F600 && codepoint_ <= 0x1F64F) ||
           (codepoint_ >= 0x1F300 && codepoint_ <= 0x1F5FF) ||
           (codepoint_ >= 0x1F680 && codepoint_ <= 0x1F6FF) ||
           (codepoint_ >= 0x1F900 && codepoint_ <= 0x1F9FF) ||
           (codepoint_ >= 0x2600 && codepoint_ <= 0x26FF) ||   // Miscellaneous Symbols
           (codepoint_ >= 0x2700 && codepoint_ <= 0x27BF);     // Dingbats
}

// === Conversion methods ===

std::string Rune::to_utf8() const {
    std::string result;

    if (codepoint_ <= 0x7F) {
        // 1-byte sequence
        result.push_back(static_cast<char>(codepoint_));
    } else if (codepoint_ <= 0x7FF) {
        // 2-byte sequence
        result.push_back(static_cast<char>(0xC0 | (codepoint_ >> 6)));
        result.push_back(static_cast<char>(0x80 | (codepoint_ & 0x3F)));
    } else if (codepoint_ <= 0xFFFF) {
        // 3-byte sequence
        result.push_back(static_cast<char>(0xE0 | (codepoint_ >> 12)));
        result.push_back(static_cast<char>(0x80 | ((codepoint_ >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (codepoint_ & 0x3F)));
    } else if (codepoint_ <= 0x10FFFF) {
        // 4-byte sequence
        result.push_back(static_cast<char>(0xF0 | (codepoint_ >> 18)));
        result.push_back(static_cast<char>(0x80 | ((codepoint_ >> 12) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | ((codepoint_ >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (codepoint_ & 0x3F)));
    } else {
        throw std::invalid_argument("Invalid Unicode codepoint");
    }

    return result;
}

std::u16string Rune::to_utf16() const {
    std::u16string result;

    if (codepoint_ <= 0xFFFF) {
        // BMP character
        result.push_back(static_cast<char16_t>(codepoint_));
    } else if (codepoint_ <= 0x10FFFF) {
        // Surrogate pair
        char32_t adjusted = codepoint_ - 0x10000;
        char16_t high = 0xD800 + (adjusted >> 10);
        char16_t low = 0xDC00 + (adjusted & 0x3FF);
        result.push_back(high);
        result.push_back(low);
    } else {
        throw std::invalid_argument("Invalid Unicode codepoint");
    }

    return result;
}

constexpr std::u32string Rune::to_utf32() const {
    return std::u32string(1, codepoint_);
}

// === Factory methods ===

Rune Rune::from_utf8(std::string_view utf8_char) {
    if (utf8_char.empty()) {
        throw std::invalid_argument("Empty UTF-8 string");
    }

    auto it = utf8_char.begin();
    char32_t codepoint = 0;

    unsigned char first = static_cast<unsigned char>(*it++);

    if (first <= 0x7F) {
        // 1-byte sequence
        codepoint = first;
    } else if ((first & 0xE0) == 0xC0) {
        // 2-byte sequence
        if (utf8_char.size() < 2) {
            throw std::invalid_argument("Invalid UTF-8 sequence");
        }
        unsigned char second = static_cast<unsigned char>(*it++);
        if ((second & 0xC0) != 0x80) {
            throw std::invalid_argument("Invalid UTF-8 sequence");
        }
        codepoint = ((first & 0x1F) << 6) | (second & 0x3F);
    } else if ((first & 0xF0) == 0xE0) {
        // 3-byte sequence
        if (utf8_char.size() < 3) {
            throw std::invalid_argument("Invalid UTF-8 sequence");
        }
        unsigned char second = static_cast<unsigned char>(*it++);
        unsigned char third = static_cast<unsigned char>(*it++);
        if ((second & 0xC0) != 0x80 || (third & 0xC0) != 0x80) {
            throw std::invalid_argument("Invalid UTF-8 sequence");
        }
        codepoint = ((first & 0x0F) << 12) | ((second & 0x3F) << 6) | (third & 0x3F);
    } else if ((first & 0xF8) == 0xF0) {
        // 4-byte sequence
        if (utf8_char.size() < 4) {
            throw std::invalid_argument("Invalid UTF-8 sequence");
        }
        unsigned char second = static_cast<unsigned char>(*it++);
        unsigned char third = static_cast<unsigned char>(*it++);
        unsigned char fourth = static_cast<unsigned char>(*it++);
        if ((second & 0xC0) != 0x80 || (third & 0xC0) != 0x80 || (fourth & 0xC0) != 0x80) {
            throw std::invalid_argument("Invalid UTF-8 sequence");
        }
        codepoint = ((first & 0x07) << 18) | ((second & 0x3F) << 12) |
                   ((third & 0x3F) << 6) | (fourth & 0x3F);
    } else {
        throw std::invalid_argument("Invalid UTF-8 sequence");
    }

    return Rune(codepoint);
}

Rune Rune::from_utf8(std::string_view utf8_char, language::Code lang) {
    Rune rune = from_utf8(utf8_char);
    rune.set_language(lang);
    return rune;
}

Rune Rune::from_utf16(std::u16string_view utf16_char) {
    if (utf16_char.empty()) {
        throw std::invalid_argument("Empty UTF-16 string");
    }

    char32_t codepoint = 0;
    char16_t first = utf16_char[0];

    if (first >= 0xD800 && first <= 0xDBFF) {
        // High surrogate
        if (utf16_char.size() < 2) {
            throw std::invalid_argument("Invalid UTF-16 surrogate pair");
        }
        char16_t second = utf16_char[1];
        if (second < 0xDC00 || second > 0xDFFF) {
            throw std::invalid_argument("Invalid UTF-16 surrogate pair");
        }
        codepoint = 0x10000 + ((first & 0x3FF) << 10) + (second & 0x3FF);
    } else if (first >= 0xDC00 && first <= 0xDFFF) {
        throw std::invalid_argument("Invalid UTF-16 sequence: unexpected low surrogate");
    } else {
        // BMP character
        codepoint = first;
    }

    return Rune(codepoint);
}

// === Language detection (constexpr) ===

constexpr language::Code Rune::detect_language(char32_t cp) noexcept {
    // ASCII range - assume English for letters
    if (cp <= 0x7F) {
        if ((cp >= U'A' && cp <= U'Z') || (cp >= U'a' && cp <= U'z')) {
            return language::Code::English;
        }
        return language::Code::Unknown;
    }

    // Korean scripts
    if ((cp >= 0x1100 && cp <= 0x11FF) ||  // Hangul Jamo
        (cp >= 0x3130 && cp <= 0x318F) ||  // Hangul Compatibility Jamo
        (cp >= 0xAC00 && cp <= 0xD7AF)) {  // Hangul Syllables
        return language::Code::Korean;
    }

    // Japanese scripts
    if ((cp >= 0x3040 && cp <= 0x309F) ||  // Hiragana
        (cp >= 0x30A0 && cp <= 0x30FF) ||  // Katakana
        (cp >= 0x31F0 && cp <= 0x31FF)) {  // Katakana Phonetic Extensions
        return language::Code::Japanese;
    }

    // CJK Ideographs (could be Chinese, Japanese, or Korean)
    if ((cp >= 0x4E00 && cp <= 0x9FFF) ||  // CJK Unified Ideographs
        (cp >= 0x3400 && cp <= 0x4DBF)) {  // CJK Unified Ideographs Extension A
        return language::Code::Chinese;  // Default to Chinese for CJK
    }

    // Latin Extended ranges - assume English/European
    if ((cp >= 0x0080 && cp <= 0x024F) ||  // Latin-1 Supplement to Latin Extended-B
        (cp >= 0x1E00 && cp <= 0x1EFF)) {  // Latin Extended Additional
        return language::Code::English;
    }

    // Cyrillic
    if (cp >= 0x0400 && cp <= 0x04FF) {
        return language::Code::Russian;
    }

    // Arabic
    if (cp >= 0x0600 && cp <= 0x06FF) {
        return language::Code::Arabic;
    }

    // Hebrew
    if (cp >= 0x0590 && cp <= 0x05FF) {
        return language::Code::Hebrew;
    }

    // Thai
    if (cp >= 0x0E00 && cp <= 0x0E7F) {
        return language::Code::Thai;
    }

        return language::Code::Unknown;
}

} // namespace rune_caster