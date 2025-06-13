#include "rune_caster/spell.hpp"
#include "rune_caster/caster.hpp"
#include <iostream>
#include <string>

using namespace rune_caster;

int main() {
    std::cout << "=== Enhanced Spell Factory Functions Demo ===\n\n";

    std::string input = "  Hello   WORLD!   This  is  a   TEST  ";
    std::cout << "Input: \"" << input << "\"\n\n";

    // 1. Factory functions for individual spells
    std::cout << "1. Individual spell factory functions:\n";
    
    auto ws_normalizer = spell::whitespace();
    auto lower_converter = spell::lowercase();
    auto upper_converter = spell::uppercase();
    auto title_converter = spell::titlecase();
    
    std::cout << "   Whitespace: \"" << spell::cast_spell(input, ws_normalizer) << "\"\n";
    std::cout << "   Lowercase: \"" << spell::cast_spell(input, lower_converter) << "\"\n";
    std::cout << "   Uppercase: \"" << spell::cast_spell(input, upper_converter) << "\"\n";
    std::cout << "   Titlecase: \"" << spell::cast_spell(input, title_converter) << "\"\n\n";

    // 2. Factory functions with parameters
    std::cout << "2. Parameterized factory functions:\n";
    
    auto no_trim_ws = spell::whitespace(true, false);  // collapse but no trim
    auto no_collapse_ws = spell::whitespace(false, true);  // trim but no collapse
    
    std::cout << "   No trim: \"" << spell::cast_spell(input, no_trim_ws) << "\"\n";
    std::cout << "   No collapse: \"" << spell::cast_spell(input, no_collapse_ws) << "\"\n\n";

    // 3. Unicode normalization factory functions
    std::cout << "3. Unicode normalization factories:\n";
    
    std::string unicode_input = "café résumé naïve";  // with accented characters
    
    auto nfc_normalizer = spell::unicode_nfc();
    auto nfd_normalizer = spell::unicode_nfd();
    auto nfkc_normalizer = spell::unicode_nfkc();
    auto nfkd_normalizer = spell::unicode_nfkd();
    
    std::cout << "   Original: \"" << unicode_input << "\"\n";
    std::cout << "   NFC: \"" << spell::cast_spell(unicode_input, nfc_normalizer) << "\"\n";
    std::cout << "   NFD: \"" << spell::cast_spell(unicode_input, nfd_normalizer) << "\"\n";
    std::cout << "   NFKC: \"" << spell::cast_spell(unicode_input, nfkc_normalizer) << "\"\n";
    std::cout << "   NFKD: \"" << spell::cast_spell(unicode_input, nfkd_normalizer) << "\"\n\n";

    // 4. Composition using factory functions
    std::cout << "4. Spell composition with factories:\n";
    
    auto composed_spell = spell::compose(spell::whitespace(), spell::titlecase());
    std::cout << "   Whitespace + Titlecase: \"" << spell::cast_spell(input, composed_spell) << "\"\n\n";

    // 5. Fluent caster pipeline with factories
    std::cout << "5. Enhanced caster pipeline:\n";
    
    auto result = make_caster(RuneSequence::from_utf8(input))
                    .cast(spell::whitespace())
                    .cast(spell::titlecase())
                    .result();
    
    std::cout << "   Pipeline result: \"" << result.to_utf8() << "\"\n\n";

    // 6. Complex multilingual example
    std::cout << "6. Multilingual processing:\n";
    
    std::string multilingual = "  Hello 안녕하세요  ΚΑΛΗΜΈΡΑ  こんにちは  ";
    
    auto multilingual_result = make_caster(RuneSequence::from_utf8(multilingual))
                                .cast(spell::whitespace())
                                .cast(spell::lowercase())
                                .result();
    
    std::cout << "   Input: \"" << multilingual << "\"\n";
    std::cout << "   Output: \"" << multilingual_result.to_utf8() << "\"\n\n";

    std::cout << "🎉 Enhanced spell factory demo completed successfully!\n";
    std::cout << "🚀 C++20 concepts ensure type safety at compile time!\n";

    return 0;
} 