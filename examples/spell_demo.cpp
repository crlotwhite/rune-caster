/**
 * @file spell_demo.cpp
 * @brief 최신 Spell 시스템의 핵심 기능을 시연하는 예제
 *
 * 이 예제는 다음 기능들을 다룹니다:
 * - 간단한 스펠 팩토리 함수들
 * - 스펠 조합 및 체이닝
 * - 캐스터 파이프라인 시스템
 * - 다국어 텍스트 처리
 * - 사전 정의된 조합 스펠
 *
 * @author Rune-Caster Development Team
 * @since 1.0.0
 */

#include "rune_caster/spell.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace rune_caster;

int main() {
    std::cout << "🔮 === Rune Caster Spell System Demo ===" << std::endl;
    std::cout << "Modern C++20 text processing with simplified spell API" << std::endl;
    std::cout << std::endl;

    try {
        // === 1. 기본 스펠 팩토리 함수들 ===
        std::cout << "🧙‍♂️ 1. Basic Spell Factory Functions:" << std::endl;

        std::string messy_text = "  Hello   WORLD!   This  is  a\tTEST\n\n  ";
        auto text_seq = RuneSequence::from_utf8(messy_text);
        std::cout << "  🔹 Input text: \"" << messy_text << "\"" << std::endl;

        // 공백 정규화
        auto ws_result = text_seq | spell::normalize_whitespace();
        std::cout << "  ✨ Whitespace normalized: \"" << ws_result.to_utf8() << "\"" << std::endl;

        // 케이스 변환
        auto lower_result = ws_result | spell::lowercase();
        std::cout << "  ✨ Lowercase: \"" << lower_result.to_utf8() << "\"" << std::endl;

        auto title_result = ws_result | spell::titlecase();
        std::cout << "  ✨ Titlecase: \"" << title_result.to_utf8() << "\"" << std::endl;

        std::cout << std::endl;

        // === 2. 파이프 연산자 체이닝 ===
        std::cout << "🔗 2. Pipe Operator Chaining:" << std::endl;

        std::string demo_text = "  programming   IS   fun  ";
        std::cout << "  🔹 Demo text: \"" << demo_text << "\"" << std::endl;

        // 파이프 연산자를 사용한 간편한 체이닝
        auto chained_result = RuneSequence::from_utf8(demo_text)
                             | spell::normalize_whitespace()
                             | spell::lowercase()
                             | spell::trim();

        std::cout << "  ✨ Chained result: \"" << chained_result.to_utf8() << "\"" << std::endl;
        std::cout << "  🔧 Chain: normalize → lowercase → trim" << std::endl;

        std::cout << std::endl;

        // === 3. 유니코드 정규화 ===
        std::cout << "🔤 3. Unicode Normalization:" << std::endl;

        std::string unicode_text = "café résumé naïve";  // 합성 문자 포함
        std::cout << "  🔹 Unicode text: \"" << unicode_text << "\"" << std::endl;

        auto nfc_result = RuneSequence::from_utf8(unicode_text) | spell::unicode_nfc();
        std::cout << "  ✨ NFC normalized: \"" << nfc_result.to_utf8() << "\"" << std::endl;

        auto nfd_result = RuneSequence::from_utf8(unicode_text) | spell::unicode_nfd();
        std::cout << "  ✨ NFD normalized: \"" << nfd_result.to_utf8() << "\"" << std::endl;

        std::cout << std::endl;

        // === 4. 캐스터 파이프라인 ===
        std::cout << "⚡ 4. Caster Pipeline (Fluent Interface):" << std::endl;

        std::string pipeline_input = "  hello   WORLD   from   RUNE-caster  ";
        std::cout << "  🔹 Pipeline input: \"" << pipeline_input << "\"" << std::endl;

        // 현대적인 파이프라인 체이닝
        auto pipeline_result = make_caster(RuneSequence::from_utf8(pipeline_input))
                               .cast(spell::normalize_whitespace())
                               .cast(spell::titlecase())
                               .cast(spell::unicode_nfc())
                               .result();

        std::cout << "  ✨ Pipeline result: \"" << pipeline_result.to_utf8() << "\"" << std::endl;
        std::cout << "  🔧 Pipeline: whitespace → titlecase → unicode(NFC)" << std::endl;

        std::cout << std::endl;

        // === 5. 다국어 처리 ===
        std::cout << "🌍 5. Multilingual Text Processing:" << std::endl;

        std::vector<std::string> test_texts = {
            "hello world",
            "안녕하세요 세계",
            "СОВРЕМЕННЫЙ C++",
            "  mixed   CASE   text  "
        };

        for (const auto& text : test_texts) {
            std::cout << "  🔹 Input: \"" << text << "\"" << std::endl;

            // 통합 파이프라인으로 처리
            auto processed = make_caster(RuneSequence::from_utf8(text))
                           .cast(spell::normalize_whitespace())
                           .cast(spell::lowercase())
                           .cast(spell::unicode_nfc())
                           .result();

            std::cout << "    ✨ Processed: \"" << processed.to_utf8() << "\"" << std::endl;
        }

        std::cout << std::endl;

        // === 6. 사전 정의된 조합 스펠 ===
        std::cout << "🎯 6. Predefined Combination Spells:" << std::endl;

        std::string combo_input = "  MESSY   input   WITH   punctuation!!!  ";
        std::cout << "  🔹 Combo input: \"" << combo_input << "\"" << std::endl;

        // 표준 정리 조합
        auto cleanup_result = RuneSequence::from_utf8(combo_input) | spell::cleanup();
        std::cout << "  ✨ Standard cleanup: \"" << cleanup_result.to_utf8() << "\"" << std::endl;
        std::cout << "    (normalize + trim + lowercase)" << std::endl;

        // 검색 전처리 조합
        auto search_result = RuneSequence::from_utf8(combo_input) | spell::search_preprocess();
        std::cout << "  ✨ Search preprocess: \"" << search_result.to_utf8() << "\"" << std::endl;
        std::cout << "    (unicode + normalize + trim + lowercase + remove punctuation)" << std::endl;

        std::cout << std::endl;

        // === 7. 성능 데모 ===
        std::cout << "🚀 7. Performance and Batch Processing:" << std::endl;

        std::vector<std::string> batch_texts = {
            "Text Processing Example 1",
            "Another Sample For Batch Processing",
            "Unicode: 한글 텍스트 배치 처리",
            "Final Example: English + 한국어 + Numbers123"
        };

        std::cout << "  🔄 Processing " << batch_texts.size() << " texts in batch:" << std::endl;

        for (size_t i = 0; i < batch_texts.size(); ++i) {
            // 최적화된 파이프라인
            auto result = make_caster(RuneSequence::from_utf8(batch_texts[i]))
                         .cast(spell::normalize_whitespace(true, true))  // 완전 정규화
                         .cast(spell::unicode_nfc())               // 유니코드 정규화
                         .cast(spell::lowercase())
                         .result();

            std::cout << "    [" << (i+1) << "] \"" << result.to_utf8() << "\"" << std::endl;
        }

        std::cout << std::endl;

        // === 8. 커스텀 스펠 생성 ===
        std::cout << "🛠️ 8. Custom Spell Creation:" << std::endl;

        std::string custom_input = "Hello123World456";
        std::cout << "  🔹 Custom input: \"" << custom_input << "\"" << std::endl;

        // 커스텀 스펠: 숫자를 밑줄로 변경
        auto digit_replacer = spell::custom("DigitReplacer", "Replace digits with underscores",
            [](const RuneSequence& input) {
                RuneSequence result;
                for (const auto& rune : input) {
                    if (rune.is_digit()) {
                        result.push_back(Rune(U'_'));
                    } else {
                        result.push_back(rune);
                    }
                }
                return result;
            });

        auto custom_result = digit_replacer(RuneSequence::from_utf8(custom_input));

        std::cout << "  ✨ Custom spell result: \"" << custom_result.to_utf8() << "\"" << std::endl;
        std::cout << "  🔧 Custom spell: " << digit_replacer.description() << std::endl;

        std::cout << std::endl;

        // === 9. 실용적인 사용 사례 ===
        std::cout << "💼 9. Practical Use Case - Text Normalization:" << std::endl;

        std::string user_input = "  User   Input:   Hello,   안녕하세요!   How ARE you TODAY?  ";
        std::cout << "  🔹 User input: \"" << user_input << "\"" << std::endl;

        // 실제 애플리케이션에서 사용할 만한 파이프라인
        auto normalized = make_caster(RuneSequence::from_utf8(user_input))
                         .cast(spell::normalize_whitespace(true, true))  // 공백 정리
                         .cast(spell::unicode_nfc())               // 유니코드 표준화
                         .result();

        std::cout << "  ✨ Normalized for storage: \"" << normalized.to_utf8() << "\"" << std::endl;

        // 표시용 변환
        auto display_version = normalized | spell::titlecase();

        std::cout << "  ✨ Formatted for display: \"" << display_version.to_utf8() << "\"" << std::endl;

        std::cout << std::endl;

        // === 10. 다양한 사용 패턴 ===
        std::cout << "🔀 10. Various Usage Patterns:" << std::endl;

        std::string pattern_input = "  Example   TEXT   for   PATTERNS  ";
        std::cout << "  🔹 Pattern input: \"" << pattern_input << "\"" << std::endl;

        // 패턴 1: 파이프 연산자
        auto pattern1 = RuneSequence::from_utf8(pattern_input)
                       | spell::trim()
                       | spell::lowercase();
        std::cout << "  ✨ Pattern 1 (pipe): \"" << pattern1.to_utf8() << "\"" << std::endl;

        // 패턴 2: 캐스터 체인
        auto pattern2 = make_caster(RuneSequence::from_utf8(pattern_input))
                       .cast(spell::normalize_whitespace())
                       .cast(spell::uppercase())
                       .result();
        std::cout << "  ✨ Pattern 2 (caster): \"" << pattern2.to_utf8() << "\"" << std::endl;

        // 패턴 3: 사전 정의된 조합
        auto pattern3 = RuneSequence::from_utf8(pattern_input) | spell::cleanup();
        std::cout << "  ✨ Pattern 3 (combo): \"" << pattern3.to_utf8() << "\"" << std::endl;

        std::cout << std::endl;
        std::cout << "🎉 === Spell System Demo Completed Successfully! ===" << std::endl;
        std::cout << "The simplified spell API is ready for production use! 🚀" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        std::cerr << "💡 Tip: Ensure proper UTF-8 encoding and valid spell usage" << std::endl;
        return 1;
    }

    return 0;
}
