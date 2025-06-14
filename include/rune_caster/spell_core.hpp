#pragma once

#include "rune_sequence.hpp"
#include "unicode.hpp"
#include "spell_base.hpp"

namespace rune_caster {
namespace spell {
namespace core {

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

    /**
     * @brief Construct a WhitespaceNormalizer
     * @param collapse_multiple If true, collapse multiple whitespace into single space
     * @param trim_edges If true, remove leading and trailing whitespace
     */
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

    /**
     * @brief Construct a UnicodeNormalizer
     * @param form The Unicode normalization form to apply
     */
    explicit UnicodeNormalizer(unicode::NormalizationForm form = unicode::NormalizationForm::NFC);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "UnicodeNormalizer"; }
};

/**
 * @brief Case conversion spell using C++20 concepts
 *
 * Converts text case according to the specified transformation.
 * Supports lowercase, uppercase, and titlecase transformations.
 */
class CaseConverter : public sequence_spell {
public:
    enum class CaseType {
        Lower,  ///< Convert to lowercase
        Upper,  ///< Convert to uppercase
        Title   ///< Convert to titlecase (first letter of each word)
    };

private:
    CaseType case_type_;

public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    /**
     * @brief Construct a CaseConverter
     * @param case_type The type of case conversion to perform
     */
    explicit CaseConverter(CaseType case_type);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "CaseConverter"; }

    /**
     * @brief Get the case type of this converter
     * @return The case conversion type
     */
    CaseType case_type() const noexcept { return case_type_; }
};

/**
 * @brief Trim leading and trailing whitespace
 */
class TrimEdges : public sequence_spell {
public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override { return "Trim leading/trailing whitespace"; }
    std::string name() const override { return "TrimEdges"; }
};

} // namespace core
} // namespace spell
} // namespace rune_caster