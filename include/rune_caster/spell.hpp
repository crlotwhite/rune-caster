#pragma once

#include <functional>
#include <string>
#include <string_view>

#include "rune.hpp"
#include "rune_sequence.hpp"
#include "language.hpp"

namespace rune_caster {
namespace spell {

/**
 * @brief Base interface for text transformation spells
 *
 * A Spell represents a transformation function that can be applied
 * to Rune or RuneSequence objects. Spells are composable and can
 * be chained together to create complex processing pipelines.
 */
template<typename InputType, typename OutputType = InputType>
class SpellInterface {
public:
    virtual ~SpellInterface() = default;

    /**
     * @brief Apply the spell transformation
     * @param input The input to transform
     * @return The transformed output
     */
    virtual OutputType operator()(const InputType& input) const = 0;

    /**
     * @brief Get a description of what this spell does
     * @return Human-readable description
     */
    virtual std::string description() const = 0;
};

/**
 * @brief Type alias for Rune transformation spells
 */
using RuneSpell = SpellInterface<Rune, Rune>;

/**
 * @brief Type alias for sequence transformation spells
 */
using SequenceSpell = SpellInterface<RuneSequence, RuneSequence>;

/**
 * @brief Type alias for sequence to string spells
 */
using StringSpell = SpellInterface<RuneSequence, std::string>;

// === Basic normalization spells ===

/**
 * @brief Normalize whitespace characters
 *
 * Converts various whitespace characters to standard space (U+0020)
 * and optionally removes extra whitespace.
 */
class WhitespaceNormalizer : public SequenceSpell {
private:
    bool collapse_multiple_;
    bool trim_edges_;

public:
    explicit WhitespaceNormalizer(bool collapse_multiple = true, bool trim_edges = true);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
};

/**
 * @brief Unicode normalization spell
 *
 * Applies Unicode normalization (NFC, NFD, NFKC, NFKD) to text.
 */
class UnicodeNormalizer : public SequenceSpell {
private:
    unicode::NormalizationForm form_;

public:
    explicit UnicodeNormalizer(unicode::NormalizationForm form = unicode::NormalizationForm::NFC);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
};

/**
 * @brief Case conversion spell
 */
class CaseConverter : public SequenceSpell {
public:
    enum class CaseType {
        Lower,
        Upper,
        Title
    };

private:
    CaseType case_type_;

public:
    explicit CaseConverter(CaseType case_type);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
};

// === Language-specific spells ===

/**
 * @brief Language detection spell
 *
 * Attempts to detect the language of text and update language hints.
 */
class LanguageDetector : public SequenceSpell {
public:
    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;

private:
    language::Code detect_language_from_script(const RuneSequence& text) const;
};

/**
 * @brief Grapheme-to-Phoneme conversion spell
 *
 * Converts textual representation to phonetic representation.
 * This is a placeholder for language-specific G2P systems.
 */
class GraphemeToPhoneme : public SequenceSpell {
private:
    language::Code target_language_;

public:
    explicit GraphemeToPhoneme(language::Code language);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;

private:
    std::string convert_to_phoneme(const Rune& rune) const;
};

// === Text filtering spells ===

/**
 * @brief Remove specific character categories
 */
class CategoryFilter : public SequenceSpell {
private:
    std::vector<unicode::Category> filtered_categories_;
    bool remove_mode_; // true = remove, false = keep only

public:
    explicit CategoryFilter(std::vector<unicode::Category> categories, bool remove = true);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
};

/**
 * @brief Remove or keep only specific scripts
 */
class ScriptFilter : public SequenceSpell {
private:
    std::vector<unicode::Script> filtered_scripts_;
    bool remove_mode_; // true = remove, false = keep only

public:
    explicit ScriptFilter(std::vector<unicode::Script> scripts, bool remove = true);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
};

// === Composition utilities ===

/**
 * @brief Compose two spells into a pipeline
 *
 * Creates a new spell that applies the first spell, then the second.
 */
template<typename IntermediateType>
class SpellComposition : public SpellInterface<RuneSequence, RuneSequence> {
private:
    std::function<IntermediateType(const RuneSequence&)> first_spell_;
    std::function<RuneSequence(const IntermediateType&)> second_spell_;
    std::string description_;

public:
    template<typename FirstSpell, typename SecondSpell>
    SpellComposition(const FirstSpell& first, const SecondSpell& second)
        : first_spell_(first)
        , second_spell_(second)
        , description_(first.description() + " → " + second.description())
    {
    }

    RuneSequence operator()(const RuneSequence& input) const override {
        return second_spell_(first_spell_(input));
    }

    std::string description() const override {
        return description_;
    }
};

// === Helper functions ===

/**
 * @brief Compose two spells
 */
template<typename FirstSpell, typename SecondSpell>
auto compose(const FirstSpell& first, const SecondSpell& second) {
    return SpellComposition<typename FirstSpell::OutputType>(first, second);
}

/**
 * @brief Apply a spell to text
 */
template<typename Spell>
RuneSequence cast_spell(const RuneSequence& input, const Spell& spell) {
    return spell(input);
}

/**
 * @brief Apply a spell to UTF-8 text
 */
template<typename Spell>
std::string cast_spell(std::string_view utf8_input, const Spell& spell) {
    auto rune_seq = RuneSequence::from_utf8(utf8_input);
    auto result = spell(rune_seq);
    return result.to_utf8();
}

} // namespace spell
} // namespace rune_caster