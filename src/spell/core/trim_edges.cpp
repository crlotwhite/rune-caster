#include "rune_caster/spell_core.hpp"

namespace rune_caster {
namespace spell {
namespace core {

RuneSequence TrimEdges::operator()(const RuneSequence& input) const {
    if (input.empty()) return RuneSequence();
    auto start = 0u;
    auto end = input.size();
    while (start < end && input[start].is_whitespace()) ++start;
    while (end > start && input[end - 1].is_whitespace()) --end;
    return input.substr(start, end - start);
}

} // namespace core
} // namespace spell
} // namespace rune_caster