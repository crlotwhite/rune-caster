#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <type_traits>

#include "rune.hpp"
#include "rune_sequence.hpp"
#include "language.hpp"
#include "spell_base.hpp"
#include "caster.hpp"

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

// === Basic normalization spells (C++20 concepts 기반) ===

/**
 * @brief Normalize whitespace characters using C++20 concepts
 *
 * Converts various whitespace characters to standard space (U+0020)
 * and optionally removes extra whitespace.
 */
class WhitespaceNormalizer : public sequence_spell {
private:
    bool collapse_multiple_;
    bool trim_edges_;

public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    explicit WhitespaceNormalizer(bool collapse_multiple = true, bool trim_edges = true);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "WhitespaceNormalizer"; }
};

/**
 * @brief Unicode normalization spell using C++20 concepts
 *
 * Applies Unicode normalization (NFC, NFD, NFKC, NFKD) to text.
 */
class UnicodeNormalizer : public sequence_spell {
private:
    unicode::NormalizationForm form_;

public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    explicit UnicodeNormalizer(unicode::NormalizationForm form = unicode::NormalizationForm::NFC);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "UnicodeNormalizer"; }
};

/**
 * @brief Case conversion spell using C++20 concepts
 */
class CaseConverter : public sequence_spell {
public:
    enum class CaseType {
        Lower,
        Upper,
        Title
    };

private:
    CaseType case_type_;

public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    explicit CaseConverter(CaseType case_type);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "CaseConverter"; }
};

// === Language-specific spells ===

/**
 * @brief Language detection spell using C++20 concepts
 *
 * Attempts to detect the language of text and update language hints.
 */
class LanguageDetector : public sequence_spell {
public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "LanguageDetector"; }

private:
    language::Code detect_language_from_script(const RuneSequence& text) const;
};

/**
 * @brief Grapheme-to-Phoneme conversion spell using C++20 concepts
 *
 * Converts textual representation to phonetic representation.
 * This is a placeholder for language-specific G2P systems.
 */
class GraphemeToPhoneme : public sequence_spell {
private:
    language::Code target_language_;

public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    explicit GraphemeToPhoneme(language::Code language);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "GraphemeToPhoneme"; }

private:
    std::string convert_to_phoneme(const Rune& rune) const;
};

// === Text filtering spells ===

/**
 * @brief Remove specific character categories using C++20 concepts
 */
class CategoryFilter : public sequence_spell {
private:
    std::vector<unicode::Category> filtered_categories_;
    bool remove_mode_; // true = remove, false = keep only

public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    explicit CategoryFilter(std::vector<unicode::Category> categories, bool remove = true);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "CategoryFilter"; }
};

/**
 * @brief Remove or keep only specific scripts using C++20 concepts
 */
class ScriptFilter : public sequence_spell {
private:
    std::vector<unicode::Script> filtered_scripts_;
    bool remove_mode_; // true = remove, false = keep only

public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    explicit ScriptFilter(std::vector<unicode::Script> scripts, bool remove = true);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "ScriptFilter"; }
};

// === Composition utilities (C++20 concepts 기반) ===

/**
 * @brief Compose two spells into a pipeline
 *
 * Creates a new spell that applies the first spell, then the second.
 */
template<typename FirstSpell, typename SecondSpell>
class SpellComposition : public sequence_spell {
private:
    FirstSpell first_spell_;
    SecondSpell second_spell_;
    std::string description_;

public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    constexpr SpellComposition(FirstSpell first, SecondSpell second)
        : first_spell_(std::move(first))
        , second_spell_(std::move(second))
        , description_(first_spell_.description() + " → " + second_spell_.description())
    {
        static_assert(std::is_same<typename FirstSpell::output_type, typename SecondSpell::input_type>::value, 
                     "First spell output must match second spell input");
    }

    RuneSequence operator()(const RuneSequence& input) const override {
        auto intermediate = first_spell_(input);
        return second_spell_(intermediate);
    }

    std::string description() const override {
        return description_;
    }

    std::string name() const override { 
        return first_spell_.name() + "+" + second_spell_.name(); 
    }
};

// === Helper functions (C++20 concepts 기반) ===

/**
 * @brief Compose two spells
 */
template<typename FirstSpell, typename SecondSpell>
auto compose(FirstSpell&& first, SecondSpell&& second) {
    return SpellComposition<std::decay_t<FirstSpell>, std::decay_t<SecondSpell>>{
        std::forward<FirstSpell>(first), 
        std::forward<SecondSpell>(second)
    };
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

// === Factory functions ===

/**
 * @brief Create a whitespace normalizer with fluent configuration
 */
inline auto whitespace() {
    return WhitespaceNormalizer{};
}

inline auto whitespace(bool collapse_multiple, bool trim_edges = true) {
    return WhitespaceNormalizer{collapse_multiple, trim_edges};
}

/**
 * @brief Create case converters
 */
inline auto lowercase() {
    return CaseConverter{CaseConverter::CaseType::Lower};
}

inline auto uppercase() {
    return CaseConverter{CaseConverter::CaseType::Upper};
}

inline auto titlecase() {
    return CaseConverter{CaseConverter::CaseType::Title};
}

/**
 * @brief Create Unicode normalizers
 */
inline auto unicode_nfc() {
    return UnicodeNormalizer{unicode::NormalizationForm::NFC};
}

inline auto unicode_nfd() {
    return UnicodeNormalizer{unicode::NormalizationForm::NFD};
}

inline auto unicode_nfkc() {
    return UnicodeNormalizer{unicode::NormalizationForm::NFKC};
}

inline auto unicode_nfkd() {
    return UnicodeNormalizer{unicode::NormalizationForm::NFKD};
}

} // namespace spell
} // namespace rune_caster