#include <iostream>
#include <string>
#include <vector>
#include "rune_caster/rune_sequence.hpp"
#include "rune_caster/spell.hpp"

using namespace rune_caster;

int main() {
    std::cout << "=== Rune Caster Pipeline Demo (C++20 Enhanced) ===" << std::endl;
    std::cout << "Version: 1.0.0" << std::endl;
    std::cout << "Description: Modern C++20 spell chaining with caster pipelines" << std::endl;
    std::cout << std::endl;

    // 1. 기본 체이닝 파이프라인 데모
    std::cout << "1. Basic Caster Pipeline Demo:" << std::endl;
    
    std::string input_text = "  Hello   WORLD!   This  is  a\tTEST\n\n  ";
    std::cout << "  Original text: \"" << input_text << "\"" << std::endl;
    
    // C++20 make_caster().cast().cast().result() 패턴
    auto sequence = RuneSequence::from_utf8(input_text);
    auto result = make_caster(sequence)
        .cast(spell::WhitespaceNormalizer{})
        .cast(spell::UnicodeNormalizer{unicode::NormalizationForm::NFC})
        .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower})
        .result();
    
    std::cout << "  Final result:  \"" << result.to_utf8() << "\"" << std::endl;
    std::cout << std::endl;

    // 2. 단계별 체이닝 시연
    std::cout << "2. Step-by-step Pipeline Visualization:" << std::endl;
    
    auto step1 = make_caster(sequence);
    std::cout << "  Step 0 (Input):     \"" << step1.result().to_utf8() << "\"" << std::endl;
    
    auto step2 = std::move(step1).cast(spell::WhitespaceNormalizer{});
    std::cout << "  Step 1 (Whitespace): \"" << step2.result().to_utf8() << "\"" << std::endl;
    
    auto step3 = std::move(step2).cast(spell::UnicodeNormalizer{});
    std::cout << "  Step 2 (Unicode):    \"" << step3.result().to_utf8() << "\"" << std::endl;
    
    auto step4 = std::move(step3).cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title});
    std::cout << "  Step 3 (Titlecase):  \"" << step4.result().to_utf8() << "\"" << std::endl;
    std::cout << std::endl;

    // 3. 다양한 케이스 변환 파이프라인
    std::cout << "3. Multiple Case Conversion Pipelines:" << std::endl;
    
    std::vector<std::string> test_texts = {
        "hello world",
        "MODERN C++ ROCKS",
        "  mixed   CASE   text  ",
        "한글과 English 混合"
    };
    
    for (const auto& text : test_texts) {
        auto seq = RuneSequence::from_utf8(text);
        
        // Lower pipeline
        auto lower_result = make_caster(seq)
            .cast(spell::WhitespaceNormalizer{})
            .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower})
            .result();
            
        // Upper pipeline  
        auto upper_result = make_caster(seq)
            .cast(spell::WhitespaceNormalizer{})
            .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Upper})
            .result();
            
        // Title pipeline
        auto title_result = make_caster(seq)
            .cast(spell::WhitespaceNormalizer{})
            .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Title})
            .result();
        
        std::cout << "  Input:     \"" << text << "\"" << std::endl;
        std::cout << "    Lower:   \"" << lower_result.to_utf8() << "\"" << std::endl;
        std::cout << "    Upper:   \"" << upper_result.to_utf8() << "\"" << std::endl;
        std::cout << "    Title:   \"" << title_result.to_utf8() << "\"" << std::endl;
        std::cout << std::endl;
    }

    // 4. 파이프라인 조합 성능 데모
    std::cout << "4. Pipeline Composition Performance Demo:" << std::endl;
    
    std::vector<std::string> batch_texts = {
        "This is text number one",
        "Another sample text for processing", 
        "Unicode: 한글 텍스트 처리 테스트",
        "Mixed content: English + 한국어 + Numbers123",
        "Final test with special characters: @#$%^&*()"
    };
    
    std::cout << "  Processing " << batch_texts.size() << " texts in batch..." << std::endl;
    
    // C++20 범위 기반 처리 (ranges는 나중에 구현)
    for (size_t i = 0; i < batch_texts.size(); ++i) {
        auto seq = RuneSequence::from_utf8(batch_texts[i]);
        
        // 완전한 정규화 파이프라인
        auto normalized = make_caster(seq)
            .cast(spell::WhitespaceNormalizer{true, true})  // 공백 정규화
            .cast(spell::UnicodeNormalizer{unicode::NormalizationForm::NFC})  // Unicode 정규화
            .cast(spell::CaseConverter{spell::CaseConverter::CaseType::Lower})  // 소문자 변환
            .result();
        
        std::cout << "    [" << (i+1) << "] \"" << normalized.to_utf8() << "\"" << std::endl;
    }
    std::cout << std::endl;

    // 5. 타입 안전성 및 컴파일 타임 체크 데모
    std::cout << "5. Type Safety and Compile-time Checking:" << std::endl;
    
    // spell_base의 타입 체크 기능 시연
    using WhitespaceSpell = spell::WhitespaceNormalizer;
    using CaseSpell = spell::CaseConverter;
    
    std::cout << "  WhitespaceNormalizer spell validation: " 
              << (is_spell_v<WhitespaceSpell> ? "✓ Valid" : "✗ Invalid") << std::endl;
    std::cout << "  CaseConverter spell validation: " 
              << (is_spell_v<CaseSpell> ? "✓ Valid" : "✗ Invalid") << std::endl;
    std::cout << "  Can chain Whitespace->Case: " 
              << (chainable_v<WhitespaceSpell, CaseSpell> ? "✓ Yes" : "✗ No") << std::endl;
    std::cout << "  Is sequence spell: " 
              << (is_sequence_spell_v<WhitespaceSpell> ? "✓ Yes" : "✗ No") << std::endl;
    
    std::cout << std::endl;
    std::cout << "🎉 Pipeline demo completed successfully!" << std::endl;
    std::cout << "The modern C++20 caster system is ready for advanced text processing!" << std::endl;
    
    return 0;
} 