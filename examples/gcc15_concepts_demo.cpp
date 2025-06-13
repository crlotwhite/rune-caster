/**
 * @file gcc15_concepts_demo.cpp
 * @brief GCC 15 C++20 concepts demonstration (compile with GCC 15)
 * 
 * This demo showcases C++20 concepts available in GCC 15.
 * Note: IDE may show errors if using Apple Clang, but compiles fine with GCC 15.
 */

#include "../include/rune_caster/spell.hpp"
#include "../include/rune_caster/caster.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace rune_caster;
using namespace rune_caster::spell;

// Simple demo that showcases the enhanced concepts system
void demo_concept_based_validation() {
    std::cout << "\n=== GCC 15 C++20 Concepts Demo ===\n";
    std::cout << "Compiler: GCC (when compiled with GCC 15)\n";
    
    // Test spell concept validation
    std::cout << "\nSpell validation with concepts:\n";
    std::cout << "WhitespaceNormalizer is valid spell: " 
              << (is_spell_v<WhitespaceNormalizer> ? "✅" : "❌") << "\n";
    std::cout << "CaseConverter is valid spell: " 
              << (is_spell_v<CaseConverter> ? "✅" : "❌") << "\n";
    
    // Test chaining validation
    std::cout << "\nChaining validation:\n";
    std::cout << "Can chain WhitespaceNormalizer -> CaseConverter: " 
              << (chainable_v<WhitespaceNormalizer, CaseConverter> ? "✅" : "❌") << "\n";
}

void demo_enhanced_pipeline() {
    std::cout << "\n=== Enhanced Pipeline with Type Safety ===\n";
    
    const std::string input = "  Hello   WORLD!   This  is  a   TEST  ";
    auto rune_input = RuneSequence::from_utf8(input);
    
         // Using the enhanced caster system with concepts
     auto result = make_caster(rune_input)
                   .cast(WhitespaceNormalizer{})
                   .cast(CaseConverter{CaseConverter::CaseType::Title})
                   .result();
    
    std::cout << "Input:  \"" << input << "\"\n";
    std::cout << "Output: \"" << result.to_utf8() << "\"\n";
    std::cout << "Type safety: ✅ (enforced by C++20 concepts)\n";
}

void demo_batch_processing() {
    std::cout << "\n=== Batch Processing with Modern C++ ===\n";
    
    std::vector<std::string> texts = {
        "  hello world  ",
        "  UPPERCASE TEXT  ",
        "  Mixed Case Text  ",
        "  한글과 English  "
    };
    
    std::cout << "Processing " << texts.size() << " texts:\n";
    
    for (size_t i = 0; i < texts.size(); ++i) {
        auto processed = make_caster(RuneSequence::from_utf8(texts[i]))
                        .cast(WhitespaceNormalizer{})
                                                 .cast(CaseConverter{CaseConverter::CaseType::Lower})
                        .result();
        
        std::cout << "  [" << (i+1) << "] \"" << texts[i] << "\" -> \"" 
                  << processed.to_utf8() << "\"\n";
    }
}

void demo_performance_measurement() {
    std::cout << "\n=== Performance with Type Safety ===\n";
    
    using namespace std::chrono;
    
    const std::string test_text = "  This is a PERFORMANCE test with Mixed Cases  ";
    auto input = RuneSequence::from_utf8(test_text);
    
    // Measure performance of the enhanced system
    auto start = high_resolution_clock::now();
    
    auto result = make_caster(input)
                  .cast(WhitespaceNormalizer{})
                                     .cast(CaseConverter{CaseConverter::CaseType::Title})
                  .result();
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    std::cout << "Input:     \"" << test_text << "\"\n";
    std::cout << "Output:    \"" << result.to_utf8() << "\"\n";
    std::cout << "Time:      " << duration.count() << " microseconds\n";
    std::cout << "Features:  C++20 concepts, perfect forwarding, move semantics\n";
}

void demo_error_prevention() {
    std::cout << "\n=== Compile-time Error Prevention ===\n";
    
    // These would cause compile-time errors with our concepts:
    std::cout << "The following would be caught at compile time:\n";
    std::cout << "1. Passing non-spell to caster.cast() -> Concept violation\n";
    std::cout << "2. Chaining incompatible spells -> Type mismatch\n";
    std::cout << "3. Invalid spell interface -> Interface validation\n";
    std::cout << "All prevented by C++20 concepts! 🛡️\n";
}

int main() {
    std::cout << "=== GCC 15 Enhanced Rune-Caster Demo ===\n";
    std::cout << "C++20 concepts, perfect forwarding, and type safety\n";
    
    try {
        demo_concept_based_validation();
        demo_enhanced_pipeline();
        demo_batch_processing();
        demo_performance_measurement();
        demo_error_prevention();
        
        std::cout << "\n🎉 GCC 15 C++20 concepts demo completed successfully!\n";
        std::cout << "Your compiler upgrade unleashed the full power of modern C++! 🚀\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 