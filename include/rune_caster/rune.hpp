#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "unicode.hpp"
#include "language.hpp"

namespace rune_caster {

/**
 * @brief Represents a single textual unit with Unicode and linguistic properties
 *
 * The Rune class encapsulates a Unicode codepoint along with its linguistic
 * attributes such as language, category, and phonetic representation.
 * It serves as the fundamental building block for text processing in the
 * rune-caster framework.
 *
 * C++20 Enhanced: Supports constexpr operations, comprehensive language detection,
 * and modern C++ features for optimal performance.
 */
class Rune {
public:
    // === Constructors (C++20 constexpr enhanced) ===

    /**
     * @brief Default constructor (creates null character)
     */
    constexpr Rune() noexcept;

    /**
     * @brief Construct a Rune from a Unicode codepoint
     * @param codepoint The Unicode codepoint
     */
    constexpr explicit Rune(char32_t codepoint) noexcept;

    /**
     * @brief Construct a Rune with explicit language information
     * @param codepoint The Unicode codepoint
     * @param language The language code
     */
    constexpr Rune(char32_t codepoint, language::Code lang) noexcept;

    /**
     * @brief Construct a Rune with phonetic representation
     * @param codepoint The Unicode codepoint
     * @param language The language code
     * @param phoneme The phonetic representation
     */
    Rune(char32_t codepoint, language::Code lang, std::string phoneme);

    // === Copy/Move semantics ===
    constexpr Rune(const Rune&) = default;
    constexpr Rune(Rune&&) noexcept = default;
    constexpr Rune& operator=(const Rune&) = default;
    constexpr Rune& operator=(Rune&&) noexcept = default;
    ~Rune() = default;

    // === Accessors (C++20 constexpr) ===

    /**
     * @brief Get the Unicode codepoint
     * @return The Unicode codepoint
     */
    [[nodiscard]] constexpr char32_t codepoint() const noexcept { return codepoint_; }

    /**
     * @brief Get the language code
     * @return The language code
     */
    [[nodiscard]] constexpr language::Code language() const noexcept { return language_; }

    /**
     * @brief Get the Unicode category
     * @return The Unicode category
     */
    [[nodiscard]] constexpr unicode::Category category() const noexcept;

    /**
     * @brief Get the Unicode script
     * @return The Unicode script
     */
    [[nodiscard]] constexpr unicode::Script script() const noexcept;

    /**
     * @brief Get the phonetic representation
     * @return The phonetic representation as a string
     */
    [[nodiscard]] const std::string& phoneme() const noexcept { return phoneme_; }

    // === Mutators ===

    /**
     * @brief Set the language code
     * @param language The new language code
     */
    constexpr void set_language(language::Code lang) noexcept { language_ = lang; }

    /**
     * @brief Set the phonetic representation
     * @param phoneme The new phonetic representation
     */
    void set_phoneme(std::string phoneme) { phoneme_ = std::move(phoneme); }

    // === Enhanced Character classification (C++20 constexpr) ===

    /**
     * @brief Check if this rune represents a vowel
     * @return true if vowel, false otherwise
     */
    [[nodiscard]] constexpr bool is_vowel() const noexcept;

    /**
     * @brief Check if this rune represents a consonant
     * @return true if consonant, false otherwise
     */
    [[nodiscard]] constexpr bool is_consonant() const noexcept;

    /**
     * @brief Check if this rune is a letter
     * @return true if letter, false otherwise
     */
    [[nodiscard]] constexpr bool is_letter() const noexcept;

    /**
     * @brief Check if this rune is a digit
     * @return true if digit, false otherwise
     */
    [[nodiscard]] constexpr bool is_digit() const noexcept;

    /**
     * @brief Check if this rune is whitespace
     * @return true if whitespace, false otherwise
     */
    [[nodiscard]] constexpr bool is_whitespace() const noexcept;

    /**
     * @brief Check if this rune is punctuation
     * @return true if punctuation, false otherwise
     */
    [[nodiscard]] constexpr bool is_punctuation() const noexcept;

    // === Language-specific properties (API Design Document requirement) ===

    /**
     * @brief Check if this is an ASCII character
     * @return true if ASCII (0-127), false otherwise
     */
    [[nodiscard]] constexpr bool is_ascii() const noexcept;

    /**
     * @brief Check if this is a Latin script character
     * @return true if Latin script, false otherwise
     */
    [[nodiscard]] constexpr bool is_latin() const noexcept;

    /**
     * @brief Check if this is a Hangul (Korean) character
     * @return true if Hangul, false otherwise
     */
    [[nodiscard]] constexpr bool is_hangul() const noexcept;

    /**
     * @brief Check if this is a Hiragana character
     * @return true if Hiragana, false otherwise
     */
    [[nodiscard]] constexpr bool is_hiragana() const noexcept;

    /**
     * @brief Check if this is a Katakana character
     * @return true if Katakana, false otherwise
     */
    [[nodiscard]] constexpr bool is_katakana() const noexcept;

    /**
     * @brief Check if this is a Kanji character
     * @return true if Kanji (CJK Unified Ideographs), false otherwise
     */
    [[nodiscard]] constexpr bool is_kanji() const noexcept;

    /**
     * @brief Check if this is an emoji character
     * @return true if emoji, false otherwise
     */
    [[nodiscard]] constexpr bool is_emoji() const noexcept;

    // === Conversion ===

    /**
     * @brief Convert to UTF-8 string
     * @return UTF-8 encoded string representation
     */
    [[nodiscard]] std::string to_utf8() const;

    /**
     * @brief Convert to UTF-16 string
     * @return UTF-16 encoded string representation
     */
    [[nodiscard]] std::u16string to_utf16() const;

    /**
     * @brief Convert to UTF-32 string
     * @return UTF-32 encoded string representation
     */
    [[nodiscard]] constexpr std::u32string to_utf32() const;

    // === Factory methods (API Design Document requirement) ===

    /**
     * @brief Create a Rune from a UTF-8 character
     * @param utf8_char UTF-8 encoded character (may be multi-byte)
     * @return Rune object
     * @throws std::invalid_argument if invalid UTF-8 sequence
     */
    static Rune from_utf8(std::string_view utf8_char);

    /**
     * @brief Create a Rune from a UTF-8 character with language hint
     * @param utf8_char UTF-8 encoded character
     * @param language Language hint for processing
     * @return Rune object
     * @throws std::invalid_argument if invalid UTF-8 sequence
     */
    static Rune from_utf8(std::string_view utf8_char, language::Code lang);

    /**
     * @brief Create a Rune from a UTF-16 character
     * @param utf16_char UTF-16 encoded character
     * @return Rune object
     * @throws std::invalid_argument if invalid UTF-16 sequence
     */
    static Rune from_utf16(std::u16string_view utf16_char);

    // === Comparison operators (C++20 three-way comparison) ===

    /**
     * @brief Three-way comparison operator (C++20)
     */
    [[nodiscard]] constexpr auto operator<=>(const Rune& other) const noexcept = default;

    /**
     * @brief Equality comparison
     */
    [[nodiscard]] constexpr bool operator==(const Rune& other) const noexcept = default;

private:
    char32_t codepoint_;        ///< Unicode codepoint
    language::Code language_;   ///< Language classification
    std::string phoneme_;       ///< Phonetic representation

    /**
     * @brief Auto-detect language from codepoint (constexpr)
     * @param cp Unicode codepoint
     * @return Detected language code
     */
    static constexpr language::Code detect_language(char32_t cp) noexcept;
};

// === User-defined literals (API Design Document requirement) ===

/**
 * @brief User-defined literal for creating Rune from Unicode codepoint
 * @param cp Unicode codepoint
 * @return Rune object
 *
 * Usage: auto rune = U'가'_rune;
 */
constexpr Rune operator""_rune(char32_t cp) noexcept {
    return Rune(cp);
}

// === Inline definitions for constructors (header-level) ===
inline constexpr Rune::Rune() noexcept : codepoint_(U'\0'), language_(language::Code::Unknown), phoneme_() {}
inline constexpr Rune::Rune(char32_t codepoint) noexcept : codepoint_(codepoint), language_(Rune::detect_language(codepoint)), phoneme_() {}
inline constexpr Rune::Rune(char32_t codepoint, language::Code lang) noexcept : codepoint_(codepoint), language_(lang), phoneme_() {}

// === Inline definitions for classification methods ===
inline constexpr bool Rune::is_letter() const noexcept { return unicode::is_letter(codepoint_); }
inline constexpr bool Rune::is_digit() const noexcept { return unicode::is_digit(codepoint_); }
inline constexpr bool Rune::is_whitespace() const noexcept { return unicode::is_whitespace(codepoint_); }
inline constexpr bool Rune::is_punctuation() const noexcept { return unicode::is_punctuation(codepoint_); }

// === Language detection (inline implementation) ===
inline constexpr language::Code Rune::detect_language(char32_t cp) noexcept {
    // Korean (Hangul)
    if ((cp >= 0x1100 && cp <= 0x11FF) ||  // Hangul Jamo
        (cp >= 0x3130 && cp <= 0x318F) ||  // Hangul Compatibility Jamo
        (cp >= 0xAC00 && cp <= 0xD7AF)) {  // Hangul Syllables
        return language::Code::Korean;
    }
    
    // Japanese
    if ((cp >= 0x3040 && cp <= 0x309F) ||  // Hiragana
        (cp >= 0x30A0 && cp <= 0x30FF) ||  // Katakana
        (cp >= 0x31F0 && cp <= 0x31FF)) {  // Katakana Phonetic Extensions
        return language::Code::Japanese;
    }
    
    // Chinese (CJK Ideographs)
    if ((cp >= 0x4E00 && cp <= 0x9FFF) ||  // CJK Unified Ideographs
        (cp >= 0x3400 && cp <= 0x4DBF) ||  // CJK Extension A
        (cp >= 0x20000 && cp <= 0x2A6DF)) { // CJK Extension B
        return language::Code::Chinese;
    }
    
    // English/Latin
    if ((cp <= 0x024F) ||                   // Basic Latin + Latin Extended
        (cp >= 0x1E00 && cp <= 0x1EFF)) {  // Latin Extended Additional
        return language::Code::English;
    }
    
    return language::Code::Unknown;
}
inline constexpr bool Rune::is_vowel() const noexcept {
    char32_t c = codepoint_;
    
    // 라틴 알파벳 모음 (영어)
    if (c == U'a' || c == U'e' || c == U'i' || c == U'o' || c == U'u' ||
        c == U'A' || c == U'E' || c == U'I' || c == U'O' || c == U'U') {
        return true;
    }
    
    // 한글 모음 (ㅏ-ㅣ 범위)
    if (c >= U'ㅏ' && c <= U'ㅣ') {
        return true;
    }
    
    // 일본어 모음 (히라가나)
    if (c == U'あ' || c == U'い' || c == U'う' || c == U'え' || c == U'お') {
        return true;
    }
    
    // 일본어 모음 (가타카나)
    if (c == U'ア' || c == U'イ' || c == U'ウ' || c == U'エ' || c == U'オ') {
        return true;
    }
    
    return false;
}
inline constexpr bool Rune::is_consonant() const noexcept {
    return is_letter() && !is_vowel();
}
// inline definitions for previously constexpr functions already specialized in rune.cpp (will be identical)

} // namespace rune_caster
