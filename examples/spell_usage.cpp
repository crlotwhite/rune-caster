#include <iostream>
#include <string>
#include "rune_caster/rune_sequence.hpp"
#include "rune_caster/spell.hpp"

using namespace rune_caster;

int main() {
    std::cout << "=== Rune Caster Spell Usage Example ===" << std::endl;
    std::cout << "Version: 1.0.0" << std::endl;
    std::cout << "Description: Text processing with Spell algorithms" << std::endl;
    std::cout << std::endl;

    // 1. 기본 공백 정제 예제
    std::cout << "1. Whitespace normalization examples:" << std::endl;
    
    // 다양한 공백이 포함된 텍스트
    std::string messy_text = "  Hello\t\tWorld   \n\n   How are you?   ";
    auto sequence = RuneSequence::from_utf8(messy_text);
    
    std::cout << "  Original text: \"" << messy_text << "\"" << std::endl;
    std::cout << "  Length: " << sequence.size() << " runes" << std::endl;
    
    // 기본 공백 정제 (축소 + 트림)
    spell::WhitespaceNormalizer basic_normalizer;
    auto cleaned = basic_normalizer(sequence);
    
    std::cout << "  After normalization: \"" << cleaned.to_utf8() << "\"" << std::endl;
    std::cout << "  Length: " << cleaned.size() << " runes" << std::endl;
    std::cout << "  Description: " << basic_normalizer.description() << std::endl;
    std::cout << std::endl;

    // 2. 옵션별 공백 처리
    std::cout << "2. Different whitespace normalization options:" << std::endl;
    
    std::string test_text = "   Start   Middle   End   ";
    auto test_sequence = RuneSequence::from_utf8(test_text);
    std::cout << "  Test text: \"" << test_text << "\"" << std::endl;
    
    // 축소만 (트림 없음)
    spell::WhitespaceNormalizer collapse_only(true, false);
    auto collapsed = collapse_only(test_sequence);
    std::cout << "  Collapse only: \"" << collapsed.to_utf8() << "\"" << std::endl;
    std::cout << "  Description: " << collapse_only.description() << std::endl;
    
    // 트림만 (축소 없음)
    spell::WhitespaceNormalizer trim_only(false, true);
    auto trimmed = trim_only(test_sequence);
    std::cout << "  Trim only: \"" << trimmed.to_utf8() << "\"" << std::endl;
    std::cout << "  Description: " << trim_only.description() << std::endl;
    
    // 둘 다 적용
    spell::WhitespaceNormalizer both(true, true);
    auto both_applied = both(test_sequence);
    std::cout << "  Both applied: \"" << both_applied.to_utf8() << "\"" << std::endl;
    std::cout << "  Description: " << both.description() << std::endl;
    std::cout << std::endl;

    // 3. 다국어 텍스트 처리
    std::cout << "3. Multilingual text processing:" << std::endl;
    
    std::string multilingual = "  안녕하세요   Hello\t\tこんにちは  ";
    auto ml_sequence = RuneSequence::from_utf8(multilingual);
    
    std::cout << "  Original: \"" << multilingual << "\"" << std::endl;
    
    spell::WhitespaceNormalizer ml_normalizer;
    auto ml_cleaned = ml_normalizer(ml_sequence);
    
    std::cout << "  Cleaned: \"" << ml_cleaned.to_utf8() << "\"" << std::endl;
    std::cout << "  Primary language: " << static_cast<int>(ml_cleaned.primary_language()) << std::endl;
    std::cout << std::endl;

    // 4. 헬퍼 함수 사용
    std::cout << "4. Using helper functions:" << std::endl;
    
    std::string helper_text = "\n\n  Text with   extra    spaces  \t\n";
    std::cout << "  Input: \"" << helper_text << "\"" << std::endl;
    
    // cast_spell 헬퍼 함수 사용
    spell::WhitespaceNormalizer helper_spell;
    std::string result = spell::cast_spell(helper_text, helper_spell);
    
    std::cout << "  Result: \"" << result << "\"" << std::endl;
    std::cout << std::endl;

    // 5. 빈 텍스트 및 특수 케이스
    std::cout << "5. Edge cases:" << std::endl;
    
    // 빈 문자열
    std::string empty_text = "";
    auto empty_result = spell::cast_spell(empty_text, spell::WhitespaceNormalizer());
    std::cout << "  Empty string result: \"" << empty_result << "\" (length: " << empty_result.length() << ")" << std::endl;
    
    // 공백만 있는 문자열
    std::string whitespace_only = "   \t\n   ";
    auto ws_result = spell::cast_spell(whitespace_only, spell::WhitespaceNormalizer());
    std::cout << "  Whitespace-only result: \"" << ws_result << "\" (length: " << ws_result.length() << ")" << std::endl;
    
    // 단일 문자
    std::string single_char = "A";
    auto single_result = spell::cast_spell(single_char, spell::WhitespaceNormalizer());
    std::cout << "  Single character result: \"" << single_result << "\"" << std::endl;

    std::cout << std::endl;
    std::cout << "=== Spell Usage Example Complete ===" << std::endl;

    return 0;
} 