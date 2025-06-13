/**
 * @file spell_demo.cpp
 * @brief 최신 Spell 시스템의 핵심 기능을 시연하는 예제
 *
 * 이 예제는 다음 기능들을 다룹니다:
 * - 기본 스펠 클래스들 (WhitespaceNormalizer, CaseConverter, UnicodeNormalizer)
 * - 스펠 팩토리 함수들
 * - 스펠 조합 및 체이닝
 * - 캐스터 파이프라인 시스템
 * - 다국어 텍스트 처리
 *
 * @author Rune-Caster Development Team
 * @since 1.0.0
 */

#include "rune_caster/spell.hpp"
#include "rune_caster/caster.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace rune_caster;

int main() {
    std::cout << "🔮 === Rune Caster Spell System Demo ===" << std::endl;
    std::cout << "Modern C++20 text processing with spells and pipelines" << std::endl;
    std::cout << std::endl;

    try {
        // === 1. 기본 스펠 클래스들 ===
        std::cout << "🧙‍♂️ 1. Basic Spell Classes:" << std::endl;

        std::string messy_text = "  Hello   WORLD!   This  is  a\tTEST\n\n  ";
        std::cout << "  🔹 Input text: \"" << messy_text << "\"" << std::endl;

        // 공백 정규화
        spell::WhitespaceNormalizer ws_normalizer;
        auto ws_result = spell::cast_spell(messy_text, ws_normalizer);
        std::cout << "  ✨ Whitespace normalized: \"" << ws_result << "\"" << std::endl;

        // 케이스 변환
        spell::CaseConverter lower_converter(spell::CaseConverter::CaseType::Lower);
        auto lower_result = spell::cast_spell(ws_result, lower_converter);
        std::cout << "  ✨ Lowercase: \"" << lower_result << "\"" << std::endl;

        spell::CaseConverter title_converter(spell::CaseConverter::CaseType::Title);
        auto title_result = spell::cast_spell(ws_result, title_converter);
        std::cout << "  ✨ Titlecase: \"" << title_result << "\"" << std::endl;

        std::cout << std::endl;

        // === 2. 팩토리 함수들 ===
        std::cout << "🏭 2. Factory Functions (Modern C++20):" << std::endl;

        std::string demo_text = "  programming   IS   fun  ";
        std::cout << "  🔹 Demo text: \"" << demo_text << "\"" << std::endl;

        // 팩토리 함수를 사용한 간편한 스펠 생성
        auto whitespace_cleaned = spell::cast_spell(demo_text, spell::whitespace());
        std::cout << "  ✨ whitespace(): \"" << whitespace_cleaned << "\"" << std::endl;

        auto lowercase_result = spell::cast_spell(whitespace_cleaned, spell::lowercase());
        std::cout << "  ✨ lowercase(): \"" << lowercase_result << "\"" << std::endl;

        auto uppercase_result = spell::cast_spell(whitespace_cleaned, spell::uppercase());
        std::cout << "  ✨ uppercase(): \"" << uppercase_result << "\"" << std::endl;

        auto titlecase_result = spell::cast_spell(whitespace_cleaned, spell::titlecase());
        std::cout << "  ✨ titlecase(): \"" << titlecase_result << "\"" << std::endl;

        std::cout << std::endl;

        // === 3. 유니코드 정규화 ===
        std::cout << "🔤 3. Unicode Normalization:" << std::endl;

        std::string unicode_text = "café résumé naïve";  // 합성 문자 포함
        std::cout << "  🔹 Unicode text: \"" << unicode_text << "\"" << std::endl;

        auto nfc_result = spell::cast_spell(unicode_text, spell::unicode_nfc());
        std::cout << "  ✨ NFC normalized: \"" << nfc_result << "\"" << std::endl;

        auto nfd_result = spell::cast_spell(unicode_text, spell::unicode_nfd());
        std::cout << "  ✨ NFD normalized: \"" << nfd_result << "\"" << std::endl;

        std::cout << std::endl;

        // === 4. 스펠 조합 ===
        std::cout << "🔗 4. Spell Composition:" << std::endl;

        std::string raw_input = "  MiXeD   CaSe   TeXt  ";
        std::cout << "  🔹 Raw input: \"" << raw_input << "\"" << std::endl;

        // 두 스펠을 조합
        auto combined_spell = spell::compose(
            spell::whitespace(),
            spell::titlecase()
        );

        auto composed_result = spell::cast_spell(raw_input, combined_spell);
        std::cout << "  ✨ Composed (whitespace + titlecase): \"" << composed_result << "\"" << std::endl;

        std::cout << std::endl;

        // === 5. 캐스터 파이프라인 ===
        std::cout << "⚡ 5. Caster Pipeline (Fluent Interface):" << std::endl;

        std::string pipeline_input = "  hello   WORLD   from   RUNE-caster  ";
        std::cout << "  🔹 Pipeline input: \"" << pipeline_input << "\"" << std::endl;

        // 현대적인 파이프라인 체이닝
        auto pipeline_result = make_caster(RuneSequence::from_utf8(pipeline_input))
                               .cast(spell::WhitespaceNormalizer{})
                               .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
                               .cast(spell::UnicodeNormalizer{})
                               .result();

        std::cout << "  ✨ Pipeline result: \"" << pipeline_result.to_utf8() << "\"" << std::endl;
        std::cout << "  🔧 Pipeline: whitespace → titlecase → unicode(NFC)" << std::endl;

        std::cout << std::endl;

        // === 6. 다국어 처리 ===
        std::cout << "🌍 6. Multilingual Text Processing:" << std::endl;

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
                           .cast(spell::WhitespaceNormalizer{})
                           .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower})
                           .cast(spell::UnicodeNormalizer{})
                           .result();

            std::cout << "    ✨ Processed: \"" << processed.to_utf8() << "\"" << std::endl;
        }

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
                         .cast(spell::WhitespaceNormalizer{true, true})  // 완전 정규화
                         .cast(spell::UnicodeNormalizer{})               // 유니코드 정규화
                         .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower})
                         .result();

            std::cout << "    [" << (i+1) << "] \"" << result.to_utf8() << "\"" << std::endl;
        }

        std::cout << std::endl;

        // === 8. 타입 안전성 데모 ===
        std::cout << "🛡️ 8. Type Safety with C++20 Concepts:" << std::endl;

        using WhitespaceSpell = spell::WhitespaceNormalizer;
        using CaseSpell = spell::CaseConverter;

        std::cout << "  🔍 Compile-time validations:" << std::endl;
        std::cout << "    - WhitespaceNormalizer is valid spell: "
                  << (is_spell_v<WhitespaceSpell> ? "✅" : "❌") << std::endl;
        std::cout << "    - CaseConverter is valid spell: "
                  << (is_spell_v<CaseSpell> ? "✅" : "❌") << std::endl;
        std::cout << "    - Can chain Whitespace→Case: "
                  << (chainable_v<WhitespaceSpell, CaseSpell> ? "✅" : "❌") << std::endl;
        std::cout << "    - Is sequence spell: "
                  << (is_sequence_spell_v<WhitespaceSpell> ? "✅" : "❌") << std::endl;

        std::cout << std::endl;

        // === 9. 실용적인 사용 사례 ===
        std::cout << "💼 9. Practical Use Case - Text Normalization:" << std::endl;

        std::string user_input = "  User   Input:   Hello,   안녕하세요!   How ARE you TODAY?  ";
        std::cout << "  🔹 User input: \"" << user_input << "\"" << std::endl;

        // 실제 애플리케이션에서 사용할 만한 파이프라인
        auto normalized = make_caster(RuneSequence::from_utf8(user_input))
                         .cast(spell::WhitespaceNormalizer{true, true})  // 공백 정리
                         .cast(spell::UnicodeNormalizer{})               // 유니코드 표준화
                         .result();

        std::cout << "  ✨ Normalized for storage: \"" << normalized.to_utf8() << "\"" << std::endl;

        // 표시용 변환
        auto display_version = make_caster(normalized)
                              .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
                              .result();

        std::cout << "  ✨ Formatted for display: \"" << display_version.to_utf8() << "\"" << std::endl;

        std::cout << std::endl;
        std::cout << "🎉 === Spell System Demo Completed Successfully! ===" << std::endl;
        std::cout << "The modern C++20 spell system is ready for production use! 🚀" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        std::cerr << "💡 Tip: Ensure proper UTF-8 encoding and valid spell usage" << std::endl;
        return 1;
    }

    return 0;
}