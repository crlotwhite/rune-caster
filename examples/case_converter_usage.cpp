/**
 * @file case_converter_usage.cpp
 * @brief CaseConverter 스펠의 케이스 변환 기능을 시연하는 예제
 *
 * 이 예제는 다음 기능들을 다룹니다:
 * - 대소문자 변환 (Lower, Upper, Title)
 * - 다국어 텍스트에서의 케이스 처리
 * - 캐스터 파이프라인과의 통합
 * - 실용적인 사용 사례
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
    std::cout << "📝 === CaseConverter Usage Example ===" << std::endl;
    std::cout << "Modern text case transformation with C++20 enhancements" << std::endl;
    std::cout << std::endl;

    try {
        // === 1. 기본 케이스 변환 ===
        std::cout << "🔤 1. Basic Case Conversions:" << std::endl;

        std::string sample_text = "Hello WORLD! This is a MiXeD CaSe Text.";
        std::cout << "  🔹 Original: \"" << sample_text << "\"" << std::endl;

        // 소문자 변환
        auto lowercase_result = spell::cast_spell(sample_text, spell::lowercase());
        std::cout << "  ✨ Lowercase: \"" << lowercase_result << "\"" << std::endl;

        // 대문자 변환
        auto uppercase_result = spell::cast_spell(sample_text, spell::uppercase());
        std::cout << "  ✨ Uppercase: \"" << uppercase_result << "\"" << std::endl;

        // 제목 케이스 변환
        auto titlecase_result = spell::cast_spell(sample_text, spell::titlecase());
        std::cout << "  ✨ Titlecase: \"" << titlecase_result << "\"" << std::endl;

        std::cout << std::endl;

        // === 2. 파이프라인과 결합 ===
        std::cout << "⚡ 2. Pipeline Integration:" << std::endl;

        std::string messy_input = "  programming   WITH   c++   IS   awesome  ";
        std::cout << "  🔹 Messy input: \"" << messy_input << "\"" << std::endl;

        // 공백 정리 + 케이스 변환 파이프라인
        auto cleaned_lower = make_caster(RuneSequence::from_utf8(messy_input))
                            .cast(spell::WhitespaceNormalizer{})
                            .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower})
                            .result();

        auto cleaned_title = make_caster(RuneSequence::from_utf8(messy_input))
                            .cast(spell::WhitespaceNormalizer{})
                            .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
                            .result();

        std::cout << "  ✨ Clean + Lowercase: \"" << cleaned_lower.to_utf8() << "\"" << std::endl;
        std::cout << "  ✨ Clean + Titlecase: \"" << cleaned_title.to_utf8() << "\"" << std::endl;

        std::cout << std::endl;

        // === 3. 다국어 텍스트 처리 ===
        std::cout << "🌍 3. Multilingual Text Processing:" << std::endl;

        std::vector<std::string> multilingual_texts = {
            "Hello World",
            "MODERN C++ PROGRAMMING",
            "mixed CASE handling",
            "Programming with 한글 and English",
            "Numbers123 and SYMBOLS!@#"
        };

        for (const auto& text : multilingual_texts) {
            std::cout << "  🔹 Input: \"" << text << "\"" << std::endl;

            auto lower_result = spell::cast_spell(text, spell::lowercase());
            auto title_result = spell::cast_spell(text, spell::titlecase());

            std::cout << "    - Lower: \"" << lower_result << "\"" << std::endl;
            std::cout << "    - Title: \"" << title_result << "\"" << std::endl;
        }

        std::cout << std::endl;

        // === 4. 실용적인 사용 사례 ===
        std::cout << "💼 4. Practical Use Cases:" << std::endl;

        // 사용자 이름 정규화
        std::string user_name = "  john   DOE  ";
        auto normalized_name = make_caster(RuneSequence::from_utf8(user_name))
                              .cast(spell::WhitespaceNormalizer{})
                              .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
                              .result();

        std::cout << "  🔹 User name normalization:" << std::endl;
        std::cout << "    Input: \"" << user_name << "\"" << std::endl;
        std::cout << "    ✨ Normalized: \"" << normalized_name.to_utf8() << "\"" << std::endl;

        // URL slug 생성
        std::string article_title = "How to Use Modern C++ Features";
        auto url_slug = make_caster(RuneSequence::from_utf8(article_title))
                       .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower})
                       .result();

        std::cout << "  🔹 URL slug generation:" << std::endl;
        std::cout << "    Title: \"" << article_title << "\"" << std::endl;
        std::cout << "    ✨ URL slug: \"" << url_slug.to_utf8() << "\"" << std::endl;

        // 데이터 표준화
        std::string database_field = "USER_PROFILE_DATA";
        auto standardized = spell::cast_spell(database_field, spell::titlecase());

        std::cout << "  🔹 Database field standardization:" << std::endl;
        std::cout << "    Field: \"" << database_field << "\"" << std::endl;
        std::cout << "    ✨ Display: \"" << standardized << "\"" << std::endl;

        std::cout << std::endl;

        // === 5. 고급 파이프라인 ===
        std::cout << "🚀 5. Advanced Pipeline Processing:" << std::endl;

        std::vector<std::string> batch_data = {
            "  customer   NAME  ",
            "PRODUCT_DESCRIPTION_field",
            "  email   ADDRESS  validation  ",
            "USER   preferences   SETTINGS"
        };

        std::cout << "  🔄 Processing " << batch_data.size() << " fields:" << std::endl;

        for (size_t i = 0; i < batch_data.size(); ++i) {
            // 완전한 정규화 파이프라인
            auto processed = make_caster(RuneSequence::from_utf8(batch_data[i]))
                           .cast(spell::WhitespaceNormalizer{true, true})  // 공백 완전 정리
                           .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})  // 제목 케이스
                           .result();

            std::cout << "    [" << (i+1) << "] \"" << batch_data[i] << "\" → \""
                      << processed.to_utf8() << "\"" << std::endl;
        }

        std::cout << std::endl;

        // === 6. 성능 및 메모리 효율성 ===
        std::cout << "⚡ 6. Performance and Memory Efficiency:" << std::endl;

        std::string large_text = "THIS IS A SAMPLE TEXT FOR PERFORMANCE TESTING WITH VARIOUS CASES";
        std::cout << "  🔹 Large text processing: \"" << large_text.substr(0, 30) << "...\"" << std::endl;

        // 효율적인 체인 처리
        auto efficient_result = make_caster(RuneSequence::from_utf8(large_text))
                               .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower})
                               .result();

        std::cout << "  ✨ Processed (" << efficient_result.size() << " characters): \""
                  << efficient_result.to_utf8().substr(0, 30) << "...\"" << std::endl;

        std::cout << "  📊 Memory usage: Move semantics ensure minimal copying" << std::endl;
        std::cout << "  📊 Type safety: C++20 concepts prevent invalid operations" << std::endl;

        std::cout << std::endl;

        // === 7. 특수 케이스 처리 ===
        std::cout << "🔍 7. Edge Cases and Special Handling:" << std::endl;

        std::vector<std::string> edge_cases = {
            "",                    // 빈 문자열
            "A",                   // 단일 문자
            "123",                 // 숫자만
            "!@#$%",              // 특수 문자만
            "한글",                // 비라틴 문자
            "MIXed123!@#한글"      // 복합 텍스트
        };

        for (const auto& test_case : edge_cases) {
            auto result = spell::cast_spell(test_case, spell::titlecase());
            std::cout << "  🔹 \"" << test_case << "\" → \"" << result << "\"" << std::endl;
        }

        std::cout << std::endl;
        std::cout << "🎉 === CaseConverter Example Completed Successfully! ===" << std::endl;
        std::cout << "Ready for production use with robust case handling! 🚀" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        std::cerr << "💡 Tip: Ensure proper UTF-8 encoding and valid text input" << std::endl;
        return 1;
    }

    return 0;
}