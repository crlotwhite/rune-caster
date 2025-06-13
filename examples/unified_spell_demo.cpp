/**
 * @file unified_spell_demo.cpp
 * @brief Comprehensive demonstration of the unified Spell system
 *
 * This example showcases the new Rune-style Spell architecture that enables:
 * - Single unified API for all text transformations
 * - Easy creation of custom spells using lambdas
 * - Seamless pipeline composition with operator|
 * - Full compatibility with the Caster system
 */

#include <iostream>
#include <string>
#include <vector>

// Include the new unified spell system
#include "spell_unified.hpp"
#include "spells.hpp"
#include "caster.hpp"

using namespace rune_caster;
using namespace rune_caster::spells;
using namespace rune_caster::spell_factory;
using namespace rune_caster::literals;

int main() {
    std::cout << "=== Rune Caster Unified Spell System Demo ===\n\n";

    // === 1. Basic predefined spells ===
    std::cout << "1. Basic Predefined Spells:\n";
    {
        std::string input = "  Hello, WORLD!  ";
        std::cout << "Input: \"" << input << "\"\n";

        // Use individual spells
        auto result1 = to_lowercase(input);
        std::cout << "Lowercase: \"" << result1 << "\"\n";

        auto result2 = whitespace_normalizer(input);
        std::cout << "Whitespace normalized: \"" << result2 << "\"\n";

        auto result3 = remove_punctuation(input);
        std::cout << "Punctuation removed: \"" << result3 << "\"\n";
    }
    std::cout << "\n";

    // === 2. Pipeline composition ===
    std::cout << "2. Pipeline Composition with operator|:\n";
    {
        std::string input = "  Hello, WORLD! How are YOU?  ";
        std::cout << "Input: \"" << input << "\"\n";

        // Compose multiple spells into a pipeline
        auto pipeline = whitespace_normalizer | to_lowercase | remove_punctuation;
        auto result = pipeline(input);

        std::cout << "Pipeline result: \"" << result << "\"\n";
        std::cout << "Pipeline description: " << pipeline.description() << "\n";
    }
    std::cout << "\n";

    // === 3. Custom spells with lambdas ===
    std::cout << "3. Custom Spells with Lambdas:\n";
    {
        std::string input = "Hello123World456";
        std::cout << "Input: \"" << input << "\"\n";

        // Create a custom spell that replaces digits with underscores
        auto digit_replacer = make_spell([](const RuneSequence& seq) -> RuneSequence {
            RuneSequence result;
            for (const auto& rune : seq) {
                if (rune.is_digit()) {
                    result.append(Rune('_'));
                } else {
                    result.append(rune);
                }
            }
            return result;
        }, "DigitReplacer", "Replace digits with underscores");

        auto result = digit_replacer(input);
        std::cout << "Custom spell result: \"" << result << "\"\n";
        std::cout << "Spell info: " << digit_replacer.to_string() << "\n";
    }
    std::cout << "\n";

    // === 4. Per-rune transformations ===
    std::cout << "4. Per-Rune Transformations:\n";
    {
        std::string input = "Hello World";
        std::cout << "Input: \"" << input << "\"\n";

        // Transform each character individually
        auto char_transformer = per_rune_spell([](const Rune& rune) -> Rune {
            if (rune.is_letter()) {
                // Alternate between upper and lower case based on position
                static int counter = 0;
                counter++;
                if (counter % 2 == 0) {
                    return Rune(std::toupper(rune.codepoint()));
                } else {
                    return Rune(std::tolower(rune.codepoint()));
                }
            }
            return rune;
        }, "AlternatingCase", "Alternate between upper and lowercase");

        auto result = char_transformer(input);
        std::cout << "Alternating case: \"" << result << "\"\n";
    }
    std::cout << "\n";

    // === 5. Filtering spells ===
    std::cout << "5. Filtering Spells:\n";
    {
        std::string input = "Hello123World456!@#";
        std::cout << "Input: \"" << input << "\"\n";

        // Keep only letters
        auto letters_only = keep_if([](const Rune& rune) {
            return rune.is_letter();
        });

        // Remove digits
        auto no_digits = remove_if([](const Rune& rune) {
            return rune.is_digit();
        });

        auto result1 = letters_only(input);
        std::cout << "Letters only: \"" << result1 << "\"\n";

        auto result2 = no_digits(input);
        std::cout << "No digits: \"" << result2 << "\"\n";
    }
    std::cout << "\n";

    // === 6. String literals ===
    std::cout << "6. String Literals:\n";
    {
        std::string input = "The old cat sat on the old mat";
        std::cout << "Input: \"" << input << "\"\n";

        // Use string literal to create replacement spell
        auto replacer = "old -> new"_spell;
        auto result = replacer(input);

        std::cout << "Replacement result: \"" << result << "\"\n";
        std::cout << "Replacer info: " << replacer.to_string() << "\n";
    }
    std::cout << "\n";

    // === 7. Complex pipeline combination ===
    std::cout << "7. Complex Pipeline Combination:\n";
    {
        std::string input = "  The Quick BROWN fox123 jumps-over THE lazy DOG!!!  ";
        std::cout << "Input: \"" << input << "\"\n";

        // Create a complex processing pipeline
        auto preprocessor = whitespace_normalizer | to_lowercase;
        auto cleaner = remove_punctuation | remove_if([](const Rune& r) { return r.is_digit(); });
        auto normalizer = unicode_nfc;

        // Combine into a comprehensive pipeline
        auto full_pipeline = preprocessor | cleaner | normalizer;

        auto result = full_pipeline(input);
        std::cout << "Full pipeline result: \"" << result << "\"\n";
        std::cout << "Pipeline depth: " << full_pipeline.composition_depth() << "\n";
        std::cout << "Is composition: " << (full_pipeline.is_composition() ? "Yes" : "No") << "\n";
    }
    std::cout << "\n";

    // === 8. Conditional spells ===
    std::cout << "8. Conditional Spells:\n";
    {
        std::vector<std::string> inputs = {
            "hello world",      // lowercase
            "HELLO WORLD",      // uppercase
            "Mixed Case Text"   // mixed
        };

        // Create a conditional spell that normalizes based on input
        auto smart_normalizer = Spell::conditional(
            [](const RuneSequence& seq) -> bool {
                // Check if input is mostly uppercase
                int upper_count = 0, letter_count = 0;
                for (const auto& rune : seq) {
                    if (rune.is_letter()) {
                        letter_count++;
                        if (std::isupper(rune.codepoint())) {
                            upper_count++;
                        }
                    }
                }
                return letter_count > 0 && (upper_count * 2 > letter_count);
            },
            to_lowercase,  // If mostly uppercase, convert to lowercase
            to_titlecase   // Otherwise, convert to titlecase
        );

        for (const auto& input : inputs) {
            std::cout << "Input: \"" << input << "\" -> \""
                     << smart_normalizer(input) << "\"\n";
        }
    }
    std::cout << "\n";

    // === 9. Caster integration ===
    std::cout << "9. Caster Integration:\n";
    {
        std::string input = "  HELLO, spell WORLD!  ";
        std::cout << "Input: \"" << input << "\"\n";

        // Use spells with Caster (conceptual - actual integration depends on Caster API)
        auto text_processor = whitespace_normalizer | to_lowercase | remove_punctuation;

        std::cout << "Note: Caster integration allows spells to be used seamlessly\n";
        std::cout << "in data processing pipelines alongside other transformations.\n";

        auto result = text_processor(input);
        std::cout << "Processed: \"" << result << "\"\n";
    }
    std::cout << "\n";

    // === 10. Performance and introspection ===
    std::cout << "10. Performance and Introspection:\n";
    {
        auto complex_spell = whitespace_normalizer | to_lowercase | remove_punctuation | unicode_nfc;

        std::cout << "Complex spell information:\n";
        std::cout << "- Name: " << complex_spell.name() << "\n";
        std::cout << "- Description: " << complex_spell.description() << "\n";
        std::cout << "- ID: " << complex_spell.id() << "\n";
        std::cout << "- Composition depth: " << complex_spell.composition_depth() << "\n";
        std::cout << "- Full details: " << complex_spell.to_string() << "\n";

        // Test the spell
        std::string test_input = "  Test INPUT!!!  ";
        auto test_result = complex_spell.test(RuneSequence::from_utf8(test_input));
        std::cout << "Test: \"" << test_input << "\" -> \"" << test_result.to_utf8() << "\"\n";
    }

    std::cout << "\n=== Demo Complete ===\n";
    std::cout << "\nKey Benefits of the Unified Spell System:\n";
    std::cout << "✓ Single, consistent API for all text transformations\n";
    std::cout << "✓ Easy custom spell creation with lambdas and functions\n";
    std::cout << "✓ Natural pipeline composition with operator|\n";
    std::cout << "✓ Full compatibility with Caster and existing systems\n";
    std::cout << "✓ Rich introspection and debugging capabilities\n";
    std::cout << "✓ Zero-overhead abstractions where possible\n";
    std::cout << "✓ Extensible architecture for future enhancements\n";

    return 0;
}