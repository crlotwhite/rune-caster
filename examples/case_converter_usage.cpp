#include <iostream>
#include <string>
#include "rune_caster/rune_sequence.hpp"
#include "rune_caster/spell.hpp"

using namespace rune_caster;

int main() {
    std::cout << "=== Rune Caster CaseConverter Usage Example ===" << std::endl;
    std::cout << "Version: 1.0.0" << std::endl;
    std::cout << "Description: Text case conversion with CaseConverter algorithms" << std::endl;
    std::cout << std::endl;

    // 1. 기본 소문자 변환 예제
    std::cout << "1. Lowercase conversion examples:" << std::endl;
    
    std::string mixed_text = "Hello WORLD! This is a MiXeD CaSe Text.";
    auto sequence = RuneSequence::from_utf8(mixed_text);
    
    std::cout << "  Original text: \"" << mixed_text << "\"" << std::endl;
    
    spell::CaseConverter lower_converter(spell::CaseConverter::CaseType::Lower);
    auto lowercased = lower_converter(sequence);
    
    std::cout << "  Lowercased: \"" << lowercased.to_utf8() << "\"" << std::endl;
    std::cout << "  Description: " << lower_converter.description() << std::endl;
    std::cout << std::endl;

    // 2. 대문자 변환 예제
    std::cout << "2. Uppercase conversion examples:" << std::endl;
    
    std::string lower_text = "hello world! how are you today?";
    auto lower_sequence = RuneSequence::from_utf8(lower_text);
    
    std::cout << "  Original text: \"" << lower_text << "\"" << std::endl;
    
    spell::CaseConverter upper_converter(spell::CaseConverter::CaseType::Upper);
    auto uppercased = upper_converter(lower_sequence);
    
    std::cout << "  Uppercased: \"" << uppercased.to_utf8() << "\"" << std::endl;
    std::cout << "  Description: " << upper_converter.description() << std::endl;
    std::cout << std::endl;

    // 3. 제목 케이스 변환 예제
    std::cout << "3. Titlecase conversion examples:" << std::endl;
    
    std::string title_text = "the quick brown fox jumps over the lazy dog";
    auto title_sequence = RuneSequence::from_utf8(title_text);
    
    std::cout << "  Original text: \"" << title_text << "\"" << std::endl;
    
    spell::CaseConverter title_converter(spell::CaseConverter::CaseType::Title);
    auto titlecased = title_converter(title_sequence);
    
    std::cout << "  Titlecased: \"" << titlecased.to_utf8() << "\"" << std::endl;
    std::cout << "  Description: " << title_converter.description() << std::endl;
    std::cout << std::endl;

    // 4. 다국어 텍스트 처리
    std::cout << "4. Multilingual text processing:" << std::endl;
    
    std::string multilingual = "Hello 안녕하세요 こんにちは WORLD 世界";
    auto ml_sequence = RuneSequence::from_utf8(multilingual);
    
    std::cout << "  Original: \"" << multilingual << "\"" << std::endl;
    
    // 소문자 변환 - ASCII만 영향받음
    spell::CaseConverter ml_lower(spell::CaseConverter::CaseType::Lower);
    auto ml_lowered = ml_lower(ml_sequence);
    std::cout << "  Lowercased: \"" << ml_lowered.to_utf8() << "\"" << std::endl;
    
    // 대문자 변환 - ASCII만 영향받음
    spell::CaseConverter ml_upper(spell::CaseConverter::CaseType::Upper);
    auto ml_uppered = ml_upper(ml_sequence);
    std::cout << "  Uppercased: \"" << ml_uppered.to_utf8() << "\"" << std::endl;
    std::cout << std::endl;

    // 5. 헬퍼 함수 사용
    std::cout << "5. Using helper functions:" << std::endl;
    
    std::string helper_text = "Programming With C++ Is FUN!";
    std::cout << "  Input: \"" << helper_text << "\"" << std::endl;
    
    // cast_spell 헬퍼 함수 사용
    spell::CaseConverter helper_lower(spell::CaseConverter::CaseType::Lower);
    std::string result_lower = spell::cast_spell(helper_text, helper_lower);
    std::cout << "  Helper lowercase: \"" << result_lower << "\"" << std::endl;
    
    spell::CaseConverter helper_title(spell::CaseConverter::CaseType::Title);
    std::string result_title = spell::cast_spell(helper_text, helper_title);
    std::cout << "  Helper titlecase: \"" << result_title << "\"" << std::endl;
    std::cout << std::endl;

    // 6. 엣지 케이스 처리
    std::cout << "6. Edge cases:" << std::endl;
    
    // 빈 문자열
    std::string empty_text = "";
    auto empty_result = spell::cast_spell(empty_text, spell::CaseConverter(spell::CaseConverter::CaseType::Upper));
    std::cout << "  Empty string result: \"" << empty_result << "\" (length: " << empty_result.length() << ")" << std::endl;
    
    // 숫자와 특수문자만
    std::string numbers_text = "123 !@# $%^";
    auto numbers_result = spell::cast_spell(numbers_text, spell::CaseConverter(spell::CaseConverter::CaseType::Lower));
    std::cout << "  Numbers/symbols result: \"" << numbers_result << "\" (unchanged)" << std::endl;
    
    // 단일 문자
    std::string single_char = "A";
    auto single_result = spell::cast_spell(single_char, spell::CaseConverter(spell::CaseConverter::CaseType::Lower));
    std::cout << "  Single character result: \"" << single_result << "\"" << std::endl;

    std::cout << std::endl;
    std::cout << "=== CaseConverter Usage Example Complete ===" << std::endl;

    return 0;
} 