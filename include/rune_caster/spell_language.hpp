#pragma once

#include "spell_base.hpp"
#include "language.hpp"
#include "rune_sequence.hpp"

namespace rune_caster {

// Forward declarations
class Rune;

namespace spell {
namespace language {

/**
 * @brief Language detection spell using C++20 concepts
 *
 * Attempts to detect the language of text and update language hints.
 * This is useful for preprocessing text before applying language-specific
 * transformations.
 */
class LanguageDetector : public sequence_spell {
public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    /**
     * @brief Apply language detection to the input sequence
     * @param input The text sequence to analyze
     * @return The same sequence with updated language hints
     */
    RuneSequence operator()(const RuneSequence& input) const override;

    std::string description() const override;
    std::string name() const override { return "LanguageDetector"; }

private:
    /**
     * @brief Detect language based on script analysis
     * @param text The text to analyze
     * @return The detected language code
     */
    rune_caster::language::Code detect_language_from_script(const RuneSequence& text) const;
};

/**
 * @brief Grapheme-to-Phoneme conversion spell using C++20 concepts
 *
 * Converts textual representation to phonetic representation.
 * This is a placeholder for language-specific G2P systems.
 *
 * Different languages require different phonetic conversion rules,
 * so this spell is configured with a target language.
 */
class GraphemeToPhoneme : public sequence_spell {
private:
    rune_caster::language::Code target_language_;

public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    /**
     * @brief Construct a G2P converter for a specific language
     * @param language The target language for phonetic conversion
     */
    explicit GraphemeToPhoneme(rune_caster::language::Code language);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "GraphemeToPhoneme"; }

    /**
     * @brief Get the target language of this converter
     * @return The language code
     */
    rune_caster::language::Code target_language() const noexcept { return target_language_; }

private:
    /**
     * @brief Convert a single rune to phonetic representation
     * @param rune The rune to convert
     * @return Phonetic representation as string
     */
    std::string convert_to_phoneme(const Rune& rune) const;
};

} // namespace language
} // namespace spell
} // namespace rune_caster
