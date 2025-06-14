#pragma once

#include "spell_base.hpp"
#include "unicode.hpp"
#include <vector>

namespace rune_caster {
namespace spell {
namespace core {

/**
 * @brief Simple whitespace tokenizer
 *        Splits input RuneSequence into tokens separated by Unicode whitespace.
 */
class WhitespaceTokenizer : public spell_base<RuneSequence, std::vector<RuneSequence>> {
public:
    using input_type = RuneSequence;
    using output_type = std::vector<RuneSequence>;

    output_type operator()(const input_type& input) const override;

    std::string description() const override { return "Tokenize on Unicode whitespace"; }
    std::string name() const override { return "WhitespaceTokenizer"; }
};

} // namespace core
} // namespace spell
} // namespace rune_caster