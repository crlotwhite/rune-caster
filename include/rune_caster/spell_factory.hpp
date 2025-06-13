#pragma once

#include "spell_core.hpp"
#include "spell_composition.hpp"
#include "unicode.hpp"

namespace rune_caster {
namespace spell {
namespace factory {

/**
 * @brief Factory functions for creating commonly used spells
 *
 * These factory functions provide convenient ways to create spell instances
 * with commonly used configurations. They use modern C++20 features for
 * optimal performance and type safety.
 */

// === Whitespace normalization factories ===

/**
 * @brief Create a standard whitespace normalizer
 *
 * Creates a WhitespaceNormalizer that collapses multiple whitespace
 * characters into single spaces and trims leading/trailing whitespace.
 *
 * @return A WhitespaceNormalizer with default settings
 */
inline auto whitespace() {
    return core::WhitespaceNormalizer{};
}

/**
 * @brief Create a customized whitespace normalizer
 *
 * @param collapse_multiple If true, collapse multiple whitespace into single space
 * @param trim_edges If true, remove leading and trailing whitespace
 * @return A WhitespaceNormalizer with custom settings
 */
inline auto whitespace(bool collapse_multiple, bool trim_edges = true) {
    return core::WhitespaceNormalizer{collapse_multiple, trim_edges};
}

// === Case conversion factories ===

/**
 * @brief Create a lowercase converter
 * @return A CaseConverter that converts text to lowercase
 */
inline auto lowercase() {
    return core::CaseConverter{core::CaseConverter::CaseType::Lower};
}

/**
 * @brief Create an uppercase converter
 * @return A CaseConverter that converts text to uppercase
 */
inline auto uppercase() {
    return core::CaseConverter{core::CaseConverter::CaseType::Upper};
}

/**
 * @brief Create a titlecase converter
 * @return A CaseConverter that converts text to titlecase
 */
inline auto titlecase() {
    return core::CaseConverter{core::CaseConverter::CaseType::Title};
}

// === Unicode normalization factories ===

/**
 * @brief Create an NFC (Canonical Decomposition followed by Canonical Composition) normalizer
 * @return A UnicodeNormalizer using NFC normalization
 */
inline auto unicode_nfc() {
    return core::UnicodeNormalizer{unicode::NormalizationForm::NFC};
}

/**
 * @brief Create an NFD (Canonical Decomposition) normalizer
 * @return A UnicodeNormalizer using NFD normalization
 */
inline auto unicode_nfd() {
    return core::UnicodeNormalizer{unicode::NormalizationForm::NFD};
}

/**
 * @brief Create an NFKC (Compatibility Decomposition followed by Canonical Composition) normalizer
 * @return A UnicodeNormalizer using NFKC normalization
 */
inline auto unicode_nfkc() {
    return core::UnicodeNormalizer{unicode::NormalizationForm::NFKC};
}

/**
 * @brief Create an NFKD (Compatibility Decomposition) normalizer
 * @return A UnicodeNormalizer using NFKD normalization
 */
inline auto unicode_nfkd() {
    return core::UnicodeNormalizer{unicode::NormalizationForm::NFKD};
}

// === Combined operation factories ===

/**
 * @brief Create a text normalizer that applies common normalizations
 *
 * This creates a composed spell that applies whitespace normalization,
 * Unicode NFC normalization, and optionally case conversion.
 *
 * @param case_type Optional case conversion to apply (default: no case conversion)
 * @return A composed spell for comprehensive text normalization
 */
template<typename CaseType = void>
auto text_normalizer(CaseType case_type = {}) {
    if constexpr (std::is_void_v<CaseType>) {
        return compose(whitespace(), unicode_nfc());
    } else {
        return compose(
            compose(whitespace(), unicode_nfc()),
            core::CaseConverter{case_type}
        );
    }
}

/**
 * @brief Create a search-optimized text preprocessor
 *
 * This creates a spell composition optimized for search indexing:
 * - Normalizes whitespace
 * - Applies Unicode NFC normalization
 * - Converts to lowercase
 *
 * @return A composed spell for search preprocessing
 */
inline auto search_preprocessor() {
    return compose(
        compose(whitespace(), unicode_nfc()),
        lowercase()
    );
}

/**
 * @brief Create a display-optimized text formatter
 *
 * This creates a spell composition optimized for display:
 * - Normalizes whitespace
 * - Applies Unicode NFC normalization
 * - Converts to titlecase
 *
 * @return A composed spell for display formatting
 */
inline auto display_formatter() {
    return compose(
        compose(whitespace(), unicode_nfc()),
        titlecase()
    );
}

} // namespace factory

// Import factory functions into the main spell namespace for convenience
using namespace factory;

} // namespace spell
} // namespace rune_caster