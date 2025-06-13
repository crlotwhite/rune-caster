#pragma once

#include "spell_unified.hpp"
#include "unicode.hpp"
#include "language.hpp"

namespace rune_caster {

/**
 * @brief Pre-defined spells using the unified Spell interface
 *
 * This namespace contains commonly used spells implemented using the new
 * unified Spell architecture. All spells are compatible with the Caster
 * pipeline and can be composed using operator|.
 */
namespace spells {

// === Text normalization spells ===

/**
 * @brief Normalize whitespace characters
 *
 * Converts various whitespace characters to standard space (U+0020)
 * and collapses multiple whitespace into single spaces.
 */
extern const Spell whitespace_normalizer;

/**
 * @brief Trim leading and trailing whitespace
 */
extern const Spell trim_whitespace;

/**
 * @brief Collapse multiple whitespace into single spaces
 */
extern const Spell collapse_whitespace;

// === Case conversion spells ===

/**
 * @brief Convert text to lowercase
 */
extern const Spell to_lowercase;

/**
 * @brief Convert text to uppercase
 */
extern const Spell to_uppercase;

/**
 * @brief Convert text to title case
 */
extern const Spell to_titlecase;

// === Unicode normalization spells ===

/**
 * @brief Apply Unicode NFC normalization
 */
extern const Spell unicode_nfc;

/**
 * @brief Apply Unicode NFD normalization
 */
extern const Spell unicode_nfd;

/**
 * @brief Apply Unicode NFKC normalization
 */
extern const Spell unicode_nfkc;

/**
 * @brief Apply Unicode NFKD normalization
 */
extern const Spell unicode_nfkd;

// === Filtering spells ===

/**
 * @brief Remove punctuation characters
 */
extern const Spell remove_punctuation;

/**
 * @brief Keep only letters and digits
 */
extern const Spell alphanumeric_only;

/**
 * @brief Remove digits
 */
extern const Spell remove_digits;

/**
 * @brief Keep only ASCII characters
 */
extern const Spell ascii_only;

// === Common combinations ===

/**
 * @brief Standard text cleanup (whitespace + NFC + lowercase)
 */
extern const Spell text_cleanup;

/**
 * @brief Search-optimized preprocessing
 */
extern const Spell search_preprocess;

/**
 * @brief Display-optimized formatting
 */
extern const Spell display_format;

// === Factory functions for dynamic spell creation ===

/**
 * @brief Create a custom whitespace normalizer
 * @param collapse_multiple If true, collapse multiple whitespace
 * @param trim_edges If true, trim leading/trailing whitespace
 * @return Custom whitespace normalizer spell
 */
Spell whitespace(bool collapse_multiple = true, bool trim_edges = true);

/**
 * @brief Create a character replacement spell
 * @param from Character to replace
 * @param to Replacement character
 * @return Character replacement spell
 */
Spell replace_char(char32_t from, char32_t to);

/**
 * @brief Create a string replacement spell
 * @param from String to replace
 * @param to Replacement string
 * @return String replacement spell
 */
Spell replace_string(std::string_view from, std::string_view to);

/**
 * @brief Create a spell that removes characters matching a predicate
 * @param predicate Function that returns true for characters to remove
 * @return Filtering spell
 */
template<typename Predicate>
Spell remove_if(Predicate&& predicate) {
    return spell_factory::filter_spell(
        [pred = std::forward<Predicate>(predicate)](const Rune& rune) {
            return !pred(rune);
        },
        "RemoveIf",
        "Remove characters matching predicate"
    );
}

/**
 * @brief Create a spell that keeps only characters matching a predicate
 * @param predicate Function that returns true for characters to keep
 * @return Filtering spell
 */
template<typename Predicate>
Spell keep_if(Predicate&& predicate) {
    return spell_factory::filter_spell(
        std::forward<Predicate>(predicate),
        "KeepIf",
        "Keep only characters matching predicate"
    );
}

/**
 * @brief Create a spell that transforms each character
 * @param transform Function that transforms individual runes
 * @return Per-character transformation spell
 */
template<typename Transform>
Spell transform_chars(Transform&& transform) {
    return spell_factory::per_rune_spell(
        std::forward<Transform>(transform),
        "TransformChars",
        "Transform each character"
    );
}

// === Language-specific spells ===

/**
 * @brief Create a language detection spell
 * @return Language detection spell
 */
Spell detect_language();

/**
 * @brief Create a script filtering spell
 * @param script The Unicode script to filter by
 * @param keep If true, keep only this script; if false, remove this script
 * @return Script filtering spell
 */
Spell filter_by_script(unicode::Script script, bool keep = true);

/**
 * @brief Create a category filtering spell
 * @param category The Unicode category to filter by
 * @param keep If true, keep only this category; if false, remove this category
 * @return Category filtering spell
 */
Spell filter_by_category(unicode::Category category, bool keep = true);

} // namespace spells

// === Usage examples and documentation ===

/**
 * @example Basic usage examples
 * @code
 * using namespace rune_caster;
 * using namespace rune_caster::spells;
 *
 * // Use predefined spells
 * auto result1 = to_lowercase("Hello World");
 *
 * // Compose spells
 * auto pipeline = whitespace_normalizer | to_lowercase | remove_punctuation;
 * auto result2 = pipeline("  Hello, World!  ");
 *
 * // Create custom spells
 * auto custom = spell_factory::make_spell([](const RuneSequence& input) {
 *     // Custom transformation logic
 *     return input;
 * }, "MySpell", "My custom transformation");
 *
 * // Use with Caster
 * auto caster_result = caster(input) | pipeline | to_string();
 *
 * // Use literals
 * using namespace rune_caster::literals;
 * auto replacer = "old -> new"_spell;
 * @endcode
 */

} // namespace rune_caster