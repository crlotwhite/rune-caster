#include <iostream>
#include <string>
#include "rune_caster/rune.hpp"
#include "rune_caster/rune_sequence.hpp"
#include "rune_caster/version.hpp"

using namespace rune_caster;

int main() {
    std::cout << "=== Rune Caster Sequence Usage Example ===\n";
    std::cout << "Version: " << version_string << "\n\n";

    try {
        // === 1. Creating RuneSequences ===
        std::cout << "1. Creating RuneSequences:\n";

        // Empty sequence
        RuneSequence empty_seq;
        std::cout << "  Empty sequence size: " << empty_seq.size() << "\n";

        // From UTF-8 text
        auto hello_seq = RuneSequence::from_utf8("Hello, 안녕하세요!");
        std::cout << "  \"Hello, 안녕하세요!\" size: " << hello_seq.size() << "\n";

        // With language hint
        auto korean_seq = RuneSequence::from_utf8("안녕하세요", language::Code::Korean);
        std::cout << "  Korean sequence primary language: "
                  << static_cast<int>(korean_seq.primary_language()) << "\n";

        // === 2. STL Container Operations ===
        std::cout << "\n2. STL Container Operations:\n";

        RuneSequence seq;
        seq.push_back(Rune(U'H'));
        seq.push_back(Rune(U'e'));
        seq.push_back(Rune(U'l'));
        seq.push_back(Rune(U'l'));
        seq.push_back(Rune(U'o'));

        std::cout << "  Built sequence size: " << seq.size() << "\n";
        std::cout << "  First character codepoint: " << static_cast<uint32_t>(seq.front().codepoint()) << "\n";
        std::cout << "  Last character codepoint: " << static_cast<uint32_t>(seq.back().codepoint()) << "\n";

        // === 3. Iteration ===
        std::cout << "\n3. Iteration:\n";
        std::cout << "  Characters in sequence: ";
        for (const auto& rune : seq) {
            std::cout << static_cast<uint32_t>(rune.codepoint()) << " ";
        }
        std::cout << "\n";

        // === 4. Subsequence operations ===
        std::cout << "\n4. Subsequence operations:\n";

        auto substr = hello_seq.substr(0, 5);
        std::cout << "  Substring (0, 5) size: " << substr.size() << "\n";
        std::cout << "  Substring to UTF-8: \"" << substr.to_utf8() << "\"\n";

        // === 5. Appending sequences ===
        std::cout << "\n5. Appending sequences:\n";

        RuneSequence combined;
        combined.append(hello_seq);
        combined.push_back(Rune(U' '));
        combined.append(korean_seq);

        std::cout << "  Combined sequence size: " << combined.size() << "\n";
        std::cout << "  Combined sequence: \"" << combined.to_utf8() << "\"\n";

        // === 6. Comparison ===
        std::cout << "\n6. Comparison:\n";

        auto hello_copy = RuneSequence::from_utf8("Hello, 안녕하세요!");
        std::cout << "  hello_seq == hello_copy: " << (hello_seq == hello_copy) << "\n";
        std::cout << "  hello_seq == korean_seq: " << (hello_seq == korean_seq) << "\n";

        // === 7. Element access ===
        std::cout << "\n7. Element access:\n";

        if (!hello_seq.empty()) {
            std::cout << "  hello_seq[0] codepoint: "
                      << static_cast<uint32_t>(hello_seq[0].codepoint()) << "\n";
            std::cout << "  hello_seq.at(1) codepoint: "
                      << static_cast<uint32_t>(hello_seq.at(1).codepoint()) << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\n=== Sequence example completed successfully! ===\n";
    return 0;
}