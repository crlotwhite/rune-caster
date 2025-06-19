// multilingual_ranges_demo.cpp: std::ranges 파이프라인으로 다국어 코퍼스 전처리 예시

#include <iostream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <utility>

#include <rune_caster/spell.hpp>

namespace rc = rune_caster;
namespace rv = std::views;

// ────────────────────────────────────────────────────────────────────────────────
// 매우 단순한 언어 감지 스텁
// 실제 프로젝트에서는 고도화된 language::detect 함수로 대체하세요.
// 여기서는 ASCII 로만 이루어진 문자열을 English 로, 그 외는 Unknown 으로 간주합니다.
// ────────────────────────────────────────────────────────────────────────────────
static rc::language::Code detect_language(const std::string& text)
{
    const bool ascii_only = std::ranges::all_of(text, [](unsigned char ch) { return ch < 0x80; });
    return ascii_only ? rc::language::Code::English : rc::language::Code::Unknown;
}

// ────────────────────────────────────────────────────────────────────────────────
// 다국어 코퍼스 전처리 함수
// ────────────────────────────────────────────────────────────────────────────────
auto process_multilingual_corpus(const std::vector<std::string>& texts)
{
    // 1) texts: std::vector<std::string>
    // 2) ASCII 검사 → (RuneSequence, lang) pair 로 변환
    // 3) Unknown 언어 제외
    // 4) spell 파이프라인(cleaning) 적용 후 RuneSequence 만 남김
    // 5) materialize to std::vector<RuneSequence>

    auto view = texts
        | rv::transform([](const std::string& txt) {
            return std::pair{ rc::RuneSequence::from_utf8(txt), detect_language(txt) };
        })
        | rv::filter([](const auto& p) {
            return p.second != rc::language::Code::Unknown;
        })
        | rv::transform([](auto&& p) {
            // spell 파이프라인: NFC 정규화 → 공백 정규화 → 구두점 제거
            return rc::make_caster(std::move(p.first))
                .cast(rc::spell::unicode_nfc())
                .cast(rc::spell::normalize_whitespace())
                .cast(rc::spell::remove_punctuation())
                .result();
        });

    return std::vector<rc::RuneSequence>(view.begin(), view.end());
}

int main()
{
    std::vector<std::string> corpus {
        "  Hello,   World!  ",
        "안녕하세요",                // 한글 → Unknown 판정(예시), 필터링됨
        "   Good-bye!!!   "
    };

    auto cleaned = process_multilingual_corpus(corpus);

    std::cout << "[Cleaned output]\n";
    for (const auto& seq : cleaned) {
        std::cout << "- '" << seq.to_utf8() << "'\n";
    }
}
