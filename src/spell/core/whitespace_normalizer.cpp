#include "rune_caster/spell.hpp"
#include "rune_caster/unicode.hpp"
#include <string>
#include <iterator>

namespace rune_caster {
namespace spell {
namespace core {

WhitespaceNormalizer::WhitespaceNormalizer(bool collapse_multiple, bool trim_edges)
    : collapse_multiple_(collapse_multiple), trim_edges_(trim_edges) {
}

RuneSequence WhitespaceNormalizer::operator()(const RuneSequence& input) const {
    if (input.empty()) {
        return RuneSequence();
    }

    RuneSequence temp_result;
    temp_result.reserve(input.size());

    // 1단계: 모든 공백을 표준 공백으로 정규화하고 collapse_multiple_ 적용
    bool prev_was_whitespace = false;

    for (const auto& rune : input) {
        if (rune.is_whitespace()) {
            // 모든 공백 문자(탭, 개행 등)를 표준 공백으로 정규화
            if (collapse_multiple_) {
                // 연속된 공백을 하나로 축소
                if (!prev_was_whitespace) {
                    temp_result.emplace_back(U' ', rune.language());
                }
            } else {
                // 공백 축소 없이 정규화만
                temp_result.emplace_back(U' ', rune.language());
            }
            prev_was_whitespace = true;
        } else {
            // 일반 문자는 그대로 추가
            temp_result.push_back(rune);
            prev_was_whitespace = false;
        }
    }

    // 2단계: trim_edges_ 적용
    if (!trim_edges_) {
        return temp_result;
    }

    // 앞쪽 공백 제거
    auto start_it = temp_result.begin();
    while (start_it != temp_result.end() && start_it->is_whitespace()) {
        ++start_it;
    }

    // 뒤쪽 공백 제거
    auto end_it = temp_result.end();
    while (end_it != start_it && (end_it - 1)->is_whitespace()) {
        --end_it;
    }

    // 결과 생성
    RuneSequence result;
    if (start_it < end_it) {
        // assign 메서드 대신 반복자를 사용해서 복사
        result.reserve(std::distance(start_it, end_it));
        for (auto it = start_it; it != end_it; ++it) {
            result.push_back(*it);
        }
    }

    return result;
}

std::string WhitespaceNormalizer::description() const {
    std::string desc = "Whitespace normalizer (";
    if (collapse_multiple_) {
        desc += "collapse multiple";
        if (trim_edges_) desc += ", ";
    }
    if (trim_edges_) {
        desc += "trim edges";
    }
    desc += ")";
    return desc;
}

} // namespace core
} // namespace spell
} // namespace rune_caster