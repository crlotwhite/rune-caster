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
 */
class Rune {
public:
        // === Constructors ===

    /**
     * @brief Default constructor (creates null character)
     */
    Rune() noexcept;

    /**
     * @brief Construct a Rune from a Unicode codepoint
     * @param codepoint The Unicode codepoint
     */
    explicit Rune(char32_t codepoint) noexcept;

        /**
     * @brief Construct a Rune with explicit language information
     * @param codepoint The Unicode codepoint
     * @param language The language code
     */
        Rune(char32_t codepoint, language::Code lang) noexcept;

    /**
     * @brief Construct a Rune with phonetic representation
     * @param codepoint The Unicode codepoint
     * @param language The language code
     * @param phoneme The phonetic representation
     */
    Rune(char32_t codepoint, language::Code lang, std::string phoneme);

    // === Copy/Move semantics ===
    Rune(const Rune&) = default;
    Rune(Rune&&) noexcept = default;
    Rune& operator=(const Rune&) = default;
    Rune& operator=(Rune&&) noexcept = default;
    ~Rune() = default;

    // === Accessors ===

    /**
     * @brief Get the Unicode codepoint
     * @return The Unicode codepoint
     */
    [[nodiscard]] char32_t codepoint() const noexcept { return codepoint_; }

    /**
     * @brief Get the language code
     * @return The language code
     */
    [[nodiscard]] language::Code language() const noexcept { return language_; }

    /**
     * @brief Get the Unicode category
     * @return The Unicode category
     */
    [[nodiscard]] unicode::Category category() const noexcept;

    /**
     * @brief Get the Unicode script
     * @return The Unicode script
     */
    [[nodiscard]] unicode::Script script() const noexcept;

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
    void set_language(language::Code lang) noexcept { language_ = lang; }

    /**
     * @brief Set the phonetic representation
     * @param phoneme The new phonetic representation
     */
    void set_phoneme(std::string phoneme) { phoneme_ = std::move(phoneme); }

    // === Character classification ===

    /**
     * @brief Check if this rune represents a vowel
     * @return true if vowel, false otherwise
     */
    [[nodiscard]] bool is_vowel() const noexcept;

    /**
     * @brief Check if this rune represents a consonant
     * @return true if consonant, false otherwise
     */
    [[nodiscard]] bool is_consonant() const noexcept;

    /**
     * @brief Check if this rune is a letter
     * @return true if letter, false otherwise
     */
    [[nodiscard]] bool is_letter() const noexcept;

    /**
     * @brief Check if this rune is a digit
     * @return true if digit, false otherwise
     */
    [[nodiscard]] bool is_digit() const noexcept;

    /**
     * @brief Check if this rune is whitespace
     * @return true if whitespace, false otherwise
     */
    [[nodiscard]] bool is_whitespace() const noexcept;

    /**
     * @brief Check if this rune is punctuation
     * @return true if punctuation, false otherwise
     */
    [[nodiscard]] bool is_punctuation() const noexcept;

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

    // === Factory methods ===

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

    // === Comparison operators ===

    /**
     * @brief Equality comparison
     */
    [[nodiscard]] bool operator==(const Rune& other) const noexcept {
        return codepoint_ == other.codepoint_;
    }

    /**
     * @brief Less than comparison for ordering
     */
    [[nodiscard]] bool operator<(const Rune& other) const noexcept {
        return codepoint_ < other.codepoint_;
    }

private:
    char32_t codepoint_;        ///< Unicode codepoint
    language::Code language_;   ///< Language classification
    std::string phoneme_;       ///< Phonetic representation

    /**
     * @brief Auto-detect language from codepoint
     * @param cp Unicode codepoint
     * @return Detected language code
     */
    static language::Code detect_language(char32_t cp) noexcept;
};

} // namespace rune_caster