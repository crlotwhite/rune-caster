#pragma once

/**
 * @file spell.hpp
 * @brief Simple and unified spell system for text transformation
 *
 * This is the ONLY header you need to include for spell functionality.
 * All spells are available through simple factory functions in the
 * rune_caster::spell namespace.
 *
 * Usage:
 * @code
 * #include <rune_caster/spell.hpp>
 * using namespace rune_caster;
 *
 * // Simple usage with pipe operator
 * auto result = text | spell::lowercase() | spell::trim();
 *
 * // Or with caster
 * auto result = make_caster(text)
 *     .cast(spell::lowercase())
 *     .cast(spell::trim())
 *     .result();
 * @endcode
 */

// Internal headers (users don't need to know about these)
#include "spell_base.hpp"
#include "spell_extensible.hpp"
#include "rune_sequence.hpp"
#include "caster.hpp"

// Implementation headers
#include "spell_core.hpp"
#include "spell_filter.hpp"
#include "spell_language.hpp"
#include "spell_token.hpp"

namespace rune_caster {
namespace spell {

// === Simple factory functions for common spells ===

/**
 * @brief Convert text to lowercase
 */
inline auto lowercase() {
    return core::CaseConverter{core::CaseConverter::CaseType::Lower};
}

/**
 * @brief Convert text to uppercase
 */
inline auto uppercase() {
    return core::CaseConverter{core::CaseConverter::CaseType::Upper};
}

/**
 * @brief Convert text to title case
 */
inline auto titlecase() {
    return core::CaseConverter{core::CaseConverter::CaseType::Title};
}

/**
 * @brief Trim leading and trailing whitespace
 */
inline auto trim() {
    return core::TrimEdges{};
}

/**
 * @brief Normalize whitespace (collapse multiple spaces, optionally trim)
 */
inline auto normalize_whitespace(bool collapse_multiple = true, bool trim_edges = true) {
    return core::WhitespaceNormalizer{collapse_multiple, trim_edges};
}

/**
 * @brief Apply Unicode NFC normalization
 */
inline auto unicode_nfc() {
    return core::UnicodeNormalizer{unicode::NormalizationForm::NFC};
}

/**
 * @brief Apply Unicode NFD normalization
 */
inline auto unicode_nfd() {
    return core::UnicodeNormalizer{unicode::NormalizationForm::NFD};
}

/**
 * @brief Apply Unicode NFKC normalization
 */
inline auto unicode_nfkc() {
    return core::UnicodeNormalizer{unicode::NormalizationForm::NFKC};
}

/**
 * @brief Apply Unicode NFKD normalization
 */
inline auto unicode_nfkd() {
    return core::UnicodeNormalizer{unicode::NormalizationForm::NFKD};
}

/**
 * @brief Tokenize text by whitespace
 */
inline auto tokenize() {
    return core::WhitespaceTokenizer{};
}

/**
 * @brief Remove punctuation characters
 */
inline auto remove_punctuation() {
    return filter::PunctuationFilter{};
}

/**
 * @brief Detect language of text
 */
inline auto detect_language() {
    return language::LanguageDetector{};
}

// === Common spell combinations ===

/**
 * @brief Standard text cleanup: normalize whitespace + trim + lowercase
 */
class TextCleanup : public spell_extensible<> {
public:
    TextCleanup() : spell_extensible{"TextCleanup", "Standard text cleanup"} {}

private:
    RuneSequence process(const RuneSequence& input) const override {
        return make_caster(input)
            .cast(spell::normalize_whitespace())
            .cast(spell::trim())
            .cast(spell::lowercase())
            .result();
    }
};

/**
 * @brief Create a standard text cleanup spell
 */
inline auto cleanup() {
    return TextCleanup{};
}

/**
 * @brief Search preprocessing: cleanup + remove punctuation + unicode NFC
 */
class SearchPreprocess : public spell_extensible<> {
public:
    SearchPreprocess() : spell_extensible{"SearchPreprocess", "Search preprocessing"} {}

private:
    RuneSequence process(const RuneSequence& input) const override {
        return make_caster(input)
            .cast(spell::unicode_nfc())
            .cast(spell::normalize_whitespace())
            .cast(spell::trim())
            .cast(spell::lowercase())
            .cast(spell::remove_punctuation())
            .result();
    }
};

/**
 * @brief Create a search preprocessing spell
 */
inline auto search_preprocess() {
    return SearchPreprocess{};
}

// === Advanced: Custom spell creation ===

/**
 * @brief Create a custom spell from a lambda function
 *
 * @code
 * auto my_spell = spell::custom("MySpell", "Description", [](const RuneSequence& input) {
 *     // Your transformation logic here
 *     return input;
 * });
 * @endcode
 */
template<typename Func>
class CustomSpell : public spell_extensible<> {
private:
    Func func_;

public:
    CustomSpell(std::string name, std::string desc, Func func)
        : spell_extensible{std::move(name), std::move(desc)}, func_{std::move(func)} {}

private:
    RuneSequence process(const RuneSequence& input) const override {
        return func_(input);
    }
};

/**
 * @brief Create a custom spell from a function
 */
template<typename Func>
auto custom(std::string name, std::string description, Func&& func) {
    return CustomSpell<std::decay_t<Func>>{
        std::move(name),
        std::move(description),
        std::forward<Func>(func)
    };
}

} // namespace spell
} // namespace rune_caster

/**
 * @brief Quick reference for available spells:
 *
 * Text case:
 * - spell::lowercase()
 * - spell::uppercase()
 * - spell::titlecase()
 *
 * Whitespace:
 * - spell::trim()
 * - spell::normalize_whitespace()
 *
 * Unicode:
 * - spell::unicode_nfc()
 * - spell::unicode_nfd()
 * - spell::unicode_nfkc()
 * - spell::unicode_nfkd()
 *
 * Tokenization:
 * - spell::tokenize()
 *
 * Filtering:
 * - spell::remove_punctuation()
 *
 * Language:
 * - spell::detect_language()
 *
 * Combinations:
 * - spell::cleanup()
 * - spell::search_preprocess()
 *
 * Custom:
 * - spell::custom(name, desc, func)
 */
