#include "rune_caster/rune.hpp"
#include <stdexcept>
#include <codecvt>
#include <locale>

namespace rune_caster {

// === Constructors ===

Rune::Rune() noexcept
    : codepoint_(U'\0')
    , language_(language::Code::Unknown)
    , phoneme_()
{
}

Rune::Rune(char32_t codepoint) noexcept
    : codepoint_(codepoint)
    , language_(detect_language(codepoint))
    , phoneme_()
{
}

Rune::Rune(char32_t codepoint, language::Code lang) noexcept
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

// === Category and Script accessors ===

unicode::Category Rune::category() const noexcept {
    return unicode::get_category(codepoint_);
}

unicode::Script Rune::script() const noexcept {
    return unicode::get_script(codepoint_);
}

// === Character classification ===

bool Rune::is_vowel() const noexcept {
    // Basic Latin vowels (extend this for other languages)
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

        default:
            return false;
    }
}

bool Rune::is_consonant() const noexcept {
    return is_letter() && !is_vowel();
}

bool Rune::is_letter() const noexcept {
    return unicode::is_letter(codepoint_);
}

bool Rune::is_digit() const noexcept {
    return unicode::is_digit(codepoint_);
}

bool Rune::is_whitespace() const noexcept {
    return unicode::is_whitespace(codepoint_);
}

bool Rune::is_punctuation() const noexcept {
    return unicode::is_punctuation(codepoint_);
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

// === Private helper methods ===

language::Code Rune::detect_language(char32_t cp) noexcept {
    if (unicode::is_hangul(cp)) {
        return language::Code::Korean;
    } else if (unicode::is_hiragana(cp) || unicode::is_katakana(cp)) {
        return language::Code::Japanese;
    } else if (unicode::is_kanji(cp)) {
        // Could be Chinese or Japanese, default to Chinese
        return language::Code::Chinese;
    } else if (unicode::is_latin(cp)) {
        // Could be many languages, default to English
        return language::Code::English;
    } else {
        return language::Code::Unknown;
    }
}

} // namespace rune_caster