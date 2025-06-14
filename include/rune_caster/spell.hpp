#pragma once

/**
 * @file spell.hpp
 * @brief Comprehensive spell system for text transformation
 *
 * This is the main header file for the Rune Caster spell system.
 * It includes all spell-related functionality organized into logical modules:
 *
 * - Core spells: Basic text normalization (whitespace, case, unicode)
 * - Language spells: Language detection and phonetic conversion
 * - Filter spells: Character category and script filtering
 * - Composition: Spell chaining and pipeline creation
 * - Factory functions: Convenient spell creation utilities
 *
 * @author Rune-Caster Development Team
 * @since 1.0.0
 */

// Core dependencies
#include "spell_base.hpp"
#include "rune_sequence.hpp"
#include "caster.hpp"

// Core spell modules
#include "spell_core.hpp"
#include "spell_language.hpp"
#include "spell_filter.hpp"
#include "spell_composition.hpp"
#include "spell_factory.hpp"
#include "spell_token.hpp"

namespace rune_caster {
namespace spell {

/**
 * @brief Main spell namespace containing all spell functionality
 *
 * This namespace provides a unified interface to all spell-related
 * functionality in the Rune Caster library. All spell classes,
 * factory functions, and utilities are available through this namespace.
 *
 * Usage examples:
 * @code
 * using namespace rune_caster::spell;
 *
 * // Use factory functions
 * auto normalizer = whitespace();
 * auto converter = lowercase();
 *
 * // Use core classes directly
 * auto custom_normalizer = core::WhitespaceNormalizer{true, false};
 *
 * // Compose spells
 * auto pipeline = compose(normalizer, converter);
 *
 * // Apply to text
 * auto result = cast_spell("  Hello WORLD  ", pipeline);
 * @endcode
 */

// === Backward compatibility aliases ===

// Import core spells into main namespace for backward compatibility
using core::WhitespaceNormalizer;
using core::UnicodeNormalizer;
using core::CaseConverter;
using core::TrimEdges;
using core::WhitespaceTokenizer;

// Import language spells
using language::LanguageDetector;
using language::GraphemeToPhoneme;

// Import filter spells
using filter::CategoryFilter;
using filter::ScriptFilter;

// === Legacy interface support ===

/**
 * @brief Legacy SpellInterface for backward compatibility
 *
 * @deprecated Use spell_base instead
 */
template<typename InputType, typename OutputType = InputType>
using SpellInterface [[deprecated("Use spell_base instead")]] = spell_base<InputType, OutputType>;

/**
 * @brief Legacy type aliases for backward compatibility
 */
using RuneSpell = spell_base<Rune, Rune>;
using SequenceSpell = spell_base<RuneSequence, RuneSequence>;
using StringSpell = spell_base<RuneSequence, std::string>;

} // namespace spell
} // namespace rune_caster

/**
 * @brief Documentation for spell system modules
 *
 * @namespace rune_caster::spell::core
 * @brief Core text normalization spells
 *
 * Contains fundamental text processing spells:
 * - WhitespaceNormalizer: Normalize whitespace characters
 * - CaseConverter: Convert text case (lower, upper, title)
 * - UnicodeNormalizer: Apply Unicode normalization forms
 *
 * @namespace rune_caster::spell::language
 * @brief Language-specific processing spells
 *
 * Contains spells for language-aware processing:
 * - LanguageDetector: Detect text language
 * - GraphemeToPhoneme: Convert text to phonetic representation
 *
 * @namespace rune_caster::spell::filter
 * @brief Text filtering and selection spells
 *
 * Contains spells for filtering text content:
 * - CategoryFilter: Filter by Unicode categories
 * - ScriptFilter: Filter by Unicode scripts
 *
 * @namespace rune_caster::spell::composition
 * @brief Spell composition and chaining utilities
 *
 * Contains utilities for combining spells:
 * - SpellComposition: Template for chaining spells
 * - compose(): Function for creating spell compositions
 * - cast_spell(): Helper functions for applying spells
 *
 * @namespace rune_caster::spell::factory
 * @brief Factory functions for common spell patterns
 *
 * Contains convenient factory functions:
 * - whitespace(), lowercase(), uppercase(), titlecase()
 * - unicode_nfc(), unicode_nfd(), unicode_nfkc(), unicode_nfkd()
 * - text_normalizer(), search_preprocessor(), display_formatter()
 */