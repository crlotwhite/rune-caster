#include "rune_caster/spell_filter.hpp"
#include "rune_caster/unicode.hpp"

namespace rune_caster {
namespace spell {
namespace filter {

RuneSequence PunctuationFilter::operator()(const RuneSequence& input) const {
    if (input.empty()) return RuneSequence();

    RuneSequence result;
    result.reserve(input.size());

    for (const auto& rune : input) {
        bool is_punct = unicode::is_punctuation(rune.codepoint());
        if (remove_mode_) {
            if (!is_punct) result.push_back(rune);
        } else {
            if (is_punct) result.push_back(rune);
        }
    }
    return result;
}

} // namespace filter
} // namespace spell
} // namespace rune_caster