/**
 * @file caster_pipeline_demo.cpp
 * @brief 최신 Caster 파이프라인 시스템의 핵심 기능을 시연하는 예제
 *
 * 이 예제는 다음 기능들을 다룹니다:
 * - 현대적인 fluent interface 파이프라인
 * - 스펠 체이닝 및 조합
 * - C++20 move semantics 최적화
 * - 실용적인 텍스트 처리 워크플로우
 *
 * @author Rune-Caster Development Team
 * @since 1.0.0
 */

#include <iostream>
#include <string>
#include <vector>
#include "rune_caster/spell.hpp"
#include "rune_caster/caster.hpp"

using namespace rune_caster;

int main() {
    std::cout << "⚡ === Caster Pipeline Demo ===" << std::endl;
    std::cout << "Modern C++20 fluent interface for text processing pipelines" << std::endl;
    std::cout << std::endl;

    try {
        // === 1. 기본 파이프라인 체이닝 ===
        std::cout << "🔗 1. Basic Pipeline Chaining:" << std::endl;

        std::string raw_input = "  Hello   WORLD!   This  is  a\tTEST\n\n  ";
        std::cout << "  🔹 Raw input: \"" << raw_input << "\"" << std::endl;

        // 단계별 파이프라인: raw input → whitespace → case → unicode → result
        auto result = make_caster(RuneSequence::from_utf8(raw_input))
                     .cast(spell::WhitespaceNormalizer{})
                     .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
                     .cast(spell::UnicodeNormalizer{})
                     .result();

        std::cout << "  ✨ Pipeline result: \"" << result.to_utf8() << "\"" << std::endl;
        std::cout << "  🔧 Steps: input → whitespace → titlecase → unicode → output" << std::endl;

        std::cout << std::endl;

        // === 2. 단계별 시각화 ===
        std::cout << "👁️ 2. Step-by-step Pipeline Visualization:" << std::endl;

        auto input_sequence = RuneSequence::from_utf8(raw_input);
        std::cout << "  🔹 Step 0 (Input):     \"" << input_sequence.to_utf8() << "\"" << std::endl;

        auto step1 = make_caster(input_sequence).cast(spell::WhitespaceNormalizer{});
        std::cout << "  🔹 Step 1 (Whitespace): \"" << step1.result().to_utf8() << "\"" << std::endl;

        auto step2 = std::move(step1).cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower});
        std::cout << "  🔹 Step 2 (Lowercase):  \"" << step2.result().to_utf8() << "\"" << std::endl;

        auto step3 = std::move(step2).cast(spell::UnicodeNormalizer{});
        std::cout << "  🔹 Step 3 (Unicode):    \"" << step3.result().to_utf8() << "\"" << std::endl;

        std::cout << std::endl;

        // === 3. 다양한 파이프라인 패턴 ===
        std::cout << "🔀 3. Different Pipeline Patterns:" << std::endl;

        std::string test_text = "  modern   C++   PROGRAMMING  ";
        std::cout << "  🔹 Test text: \"" << test_text << "\"" << std::endl;

        // 패턴 1: 정규화 → 소문자
        auto pattern1 = make_caster(RuneSequence::from_utf8(test_text))
                       .cast(spell::WhitespaceNormalizer{})
                       .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower})
                       .result();

        // 패턴 2: 정규화 → 대문자
        auto pattern2 = make_caster(RuneSequence::from_utf8(test_text))
                       .cast(spell::WhitespaceNormalizer{})
                       .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Upper})
                       .result();

        // 패턴 3: 정규화 → 제목 케이스
        auto pattern3 = make_caster(RuneSequence::from_utf8(test_text))
                       .cast(spell::WhitespaceNormalizer{})
                       .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
                       .result();

        std::cout << "  ✨ Pattern 1 (Lower):  \"" << pattern1.to_utf8() << "\"" << std::endl;
        std::cout << "  ✨ Pattern 2 (Upper):  \"" << pattern2.to_utf8() << "\"" << std::endl;
        std::cout << "  ✨ Pattern 3 (Title):  \"" << pattern3.to_utf8() << "\"" << std::endl;

        std::cout << std::endl;

        // === 4. 배치 처리 파이프라인 ===
        std::cout << "📦 4. Batch Processing Pipeline:" << std::endl;

        std::vector<std::string> batch_texts = {
            "User  Input  Data",
            "DATABASE_FIELD_NAME",
            "  configuration   SETTINGS  ",
            "API_RESPONSE_handler"
        };

        std::cout << "  🔄 Processing " << batch_texts.size() << " items in batch:" << std::endl;

        for (size_t i = 0; i < batch_texts.size(); ++i) {
            // 통일된 파이프라인 적용
            auto processed = make_caster(RuneSequence::from_utf8(batch_texts[i]))
                           .cast(spell::WhitespaceNormalizer{true, true})  // 완전 공백 정리
                           .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})  // 제목 케이스
                           .cast(spell::UnicodeNormalizer{})  // 유니코드 정규화
                           .result();

            std::cout << "    [" << (i+1) << "] \"" << batch_texts[i] << "\" → \""
                      << processed.to_utf8() << "\"" << std::endl;
        }

        std::cout << std::endl;

        // === 5. 실용적인 워크플로우 ===
        std::cout << "💼 5. Practical Workflow Examples:" << std::endl;

        // 사용자 입력 정규화 워크플로우
        std::string user_input = "  John   DOE  ";
        auto normalized_user = make_caster(RuneSequence::from_utf8(user_input))
                              .cast(spell::WhitespaceNormalizer{})
                              .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
                              .result();

        std::cout << "  🔹 User input normalization:" << std::endl;
        std::cout << "    Input: \"" << user_input << "\" → Output: \"" << normalized_user.to_utf8() << "\"" << std::endl;

        // 검색어 정규화 워크플로우
        std::string search_query = "  SEARCH   for   MODERN   c++  ";
        auto normalized_search = make_caster(RuneSequence::from_utf8(search_query))
                                .cast(spell::WhitespaceNormalizer{})
                                .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower})
                                .result();

        std::cout << "  🔹 Search query normalization:" << std::endl;
        std::cout << "    Query: \"" << search_query << "\" → Normalized: \"" << normalized_search.to_utf8() << "\"" << std::endl;

        // 데이터베이스 필드 표준화 워크플로우
        std::string db_field = "USER_PROFILE_data";
        auto standardized_field = make_caster(RuneSequence::from_utf8(db_field))
                                 .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
                                 .result();

        std::cout << "  🔹 Database field standardization:" << std::endl;
        std::cout << "    Field: \"" << db_field << "\" → Display: \"" << standardized_field.to_utf8() << "\"" << std::endl;

        std::cout << std::endl;

        // === 6. 고급 체이닝과 성능 ===
        std::cout << "🚀 6. Advanced Chaining and Performance:" << std::endl;

        std::string complex_text = "  Complex   TEXT   with   VARIOUS   formatting   ISSUES  ";
        std::cout << "  🔹 Complex input: \"" << complex_text << "\"" << std::endl;

        // 고급 체인: 모든 정규화 단계 포함
        auto advanced_result = make_caster(RuneSequence::from_utf8(complex_text))
                              .cast(spell::WhitespaceNormalizer{true, true})     // 공백 완전 정리
                              .cast(spell::UnicodeNormalizer{})                 // 유니코드 정규화
                              .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})  // 제목 케이스
                              .result();

        std::cout << "  ✨ Advanced pipeline: \"" << advanced_result.to_utf8() << "\"" << std::endl;
        std::cout << "  🔧 Chain: whitespace(full) → unicode(NFC) → titlecase" << std::endl;
        std::cout << "  📊 Performance: Zero-copy operations with move semantics" << std::endl;

        std::cout << std::endl;

        // === 7. 다국어 파이프라인 ===
        std::cout << "🌍 7. Multilingual Pipeline Processing:" << std::endl;

        std::vector<std::string> multilingual_inputs = {
            "hello world",
            "안녕하세요 세상",
            "СОВРЕМЕННЫЙ мир",
            "  mixed   언어   TEXT  "
        };

        for (const auto& input : multilingual_inputs) {
            auto result = make_caster(RuneSequence::from_utf8(input))
                         .cast(spell::WhitespaceNormalizer{})
                         .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
                         .result();

            std::cout << "  🔹 \"" << input << "\" → \"" << result.to_utf8() << "\"" << std::endl;
        }

        std::cout << std::endl;

        // === 8. 타입 안전성과 컴파일 타임 검증 ===
        std::cout << "🛡️ 8. Type Safety and Compile-time Validation:" << std::endl;

        std::cout << "  🔍 C++20 Features in use:" << std::endl;
        std::cout << "    - Concepts: Compile-time spell validation" << std::endl;
        std::cout << "    - Move semantics: Zero-copy pipeline operations" << std::endl;
        std::cout << "    - Perfect forwarding: Efficient parameter passing" << std::endl;
        std::cout << "    - Auto type deduction: Type-safe transformations" << std::endl;

        using WS = spell::WhitespaceNormalizer;
        using CC = spell::CaseConverter;

        std::cout << "  📋 Compile-time checks:" << std::endl;
        std::cout << "    - WhitespaceNormalizer is valid spell: " << (is_spell_v<WS> ? "✅" : "❌") << std::endl;
        std::cout << "    - CaseConverter is valid spell: " << (is_spell_v<CC> ? "✅" : "❌") << std::endl;
        std::cout << "    - Spells are chainable: " << (chainable_v<WS, CC> ? "✅" : "❌") << std::endl;

        std::cout << std::endl;

        // === 9. 메모리 효율성 데모 ===
        std::cout << "💾 9. Memory Efficiency Demonstration:" << std::endl;

        std::string large_input = "This is a longer text input for testing memory efficiency with multiple pipeline operations";
        std::cout << "  🔹 Large input (" << large_input.length() << " chars): \"" << large_input.substr(0, 40) << "...\"" << std::endl;

        // 메모리 효율적인 파이프라인 (move semantics 활용)
        auto memory_efficient = make_caster(RuneSequence::from_utf8(std::move(large_input)))
                               .cast(spell::WhitespaceNormalizer{})
                               .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower})
                               .result();

        std::cout << "  ✨ Processed (" << memory_efficient.size() << " runes): \""
                  << memory_efficient.to_utf8().substr(0, 40) << "...\"" << std::endl;
        std::cout << "  📊 Memory: Move semantics minimize allocations and copies" << std::endl;

        std::cout << std::endl;
        std::cout << "🎉 === Caster Pipeline Demo Completed Successfully! ===" << std::endl;
        std::cout << "Modern C++20 pipeline system ready for production! 🚀" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        std::cerr << "💡 Tip: Ensure proper UTF-8 encoding and valid pipeline operations" << std::endl;
        return 1;
    }

    return 0;
}