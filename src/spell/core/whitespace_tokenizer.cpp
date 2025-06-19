#include "rune_caster/spell_token.hpp"

namespace rune_caster {
namespace spell {
namespace core {

WhitespaceTokenizer::output_type WhitespaceTokenizer::operator()(const input_type& input) const {
    output_type tokens;
    if (input.empty()) return tokens;

    RuneSequence current;
    for (const auto& rune : input) {
        if (unicode::is_whitespace(rune.codepoint())) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current.push_back(rune);
        }
    }
    if (!current.empty()) tokens.push_back(current);
    return tokens;
}

} // namespace core
} // namespace spell
} // namespace rune_caster
