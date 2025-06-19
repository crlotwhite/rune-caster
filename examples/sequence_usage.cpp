/**
 * @file sequence_usage.cpp
 * @brief RuneSequence 컨테이너의 핵심 기능을 시연하는 예제
 *
 * 이 예제는 다음 기능들을 다룹니다:
 * - RuneSequence 생성 및 초기화
 * - STL 컨테이너 인터페이스 활용
 * - 다국어 텍스트 처리
 * - 시퀀스 조작 및 변환
 *
 * @author Rune-Caster Development Team
 * @since 1.0.0
 */

#include <iostream>
#include <string>
#include "rune_caster/rune.hpp"
#include "rune_caster/rune_sequence.hpp"
#include "rune_caster/version.hpp"

using namespace rune_caster;

int main() {
    std::cout << "🔗 === RuneSequence Usage Example ===" << std::endl;
    std::cout << "Version: " << version_string << std::endl;
    std::cout << std::endl;

    try {
        // === 1. 기본 생성 및 초기화 ===
        std::cout << "📝 1. Creating and initializing RuneSequences:" << std::endl;

        // 빈 시퀀스
        RuneSequence empty_seq;
        std::cout << "  🔹 Empty sequence size: " << empty_seq.size() << std::endl;

        // UTF-8에서 생성
        auto text_seq = RuneSequence::from_utf8("Hello, World! 안녕하세요! 🌍");
        std::cout << "  🔹 Text sequence size: " << text_seq.size() << " runes" << std::endl;
        std::cout << "  🔹 UTF-8 representation: \"" << text_seq.to_utf8() << "\"" << std::endl;

        // 언어 힌트와 함께 생성
        auto korean_seq = RuneSequence::from_utf8("한글 텍스트", language::Code::Korean);
        std::cout << "  🔹 Korean sequence primary language: "
                  << static_cast<int>(korean_seq.primary_language()) << std::endl;

        std::cout << std::endl;

        // === 2. STL 컨테이너 연산 ===
        std::cout << "🔧 2. STL container operations:" << std::endl;

        RuneSequence build_seq;

        // 개별 문자 추가
        build_seq.push_back(Rune(U'H'));
        build_seq.push_back(Rune(U'e'));
        build_seq.push_back(Rune(U'l'));
        build_seq.push_back(Rune(U'l'));
        build_seq.push_back(Rune(U'o'));

        std::cout << "  🔹 Built sequence: \"" << build_seq.to_utf8() << "\"" << std::endl;
        std::cout << "  🔹 Size: " << build_seq.size() << " runes" << std::endl;
        std::cout << "  🔹 First character: '" << build_seq.front().to_utf8() << "'" << std::endl;
        std::cout << "  🔹 Last character: '" << build_seq.back().to_utf8() << "'" << std::endl;

        std::cout << std::endl;

        // === 3. 반복자 및 범위 기반 루프 ===
        std::cout << "🔄 3. Iteration and range-based loops:" << std::endl;

        auto demo_text = RuneSequence::from_utf8("ABC가나다");
        std::cout << "  🔹 Demo text: \"" << demo_text.to_utf8() << "\"" << std::endl;
        std::cout << "  🔹 Character by character:" << std::endl;

        for (size_t i = 0; i < demo_text.size(); ++i) {
            const auto& rune = demo_text[i];
            std::cout << "    [" << i << "] '" << rune.to_utf8()
                      << "' (U+" << std::hex << static_cast<uint32_t>(rune.codepoint()) << std::dec << ")" << std::endl;
        }

        std::cout << std::endl;

        // === 4. 부분 문자열 및 조작 ===
        std::cout << "✂️ 4. Substring and manipulation:" << std::endl;

        auto long_text = RuneSequence::from_utf8("The quick brown fox jumps over the lazy dog");
        std::cout << "  🔹 Original: \"" << long_text.to_utf8() << "\"" << std::endl;

        // 부분 문자열 추출
        auto substr1 = long_text.substr(0, 9);  // "The quick"
        auto substr2 = long_text.substr(10, 5); // "brown"

        std::cout << "  🔹 Substring (0, 9): \"" << substr1.to_utf8() << "\"" << std::endl;
        std::cout << "  🔹 Substring (10, 5): \"" << substr2.to_utf8() << "\"" << std::endl;

        std::cout << std::endl;

        // === 5. 시퀀스 병합 및 조합 ===
        std::cout << "🔗 5. Sequence concatenation and composition:" << std::endl;

        RuneSequence greeting = RuneSequence::from_utf8("Hello ");
        RuneSequence target = RuneSequence::from_utf8("World");
        RuneSequence punctuation = RuneSequence::from_utf8("!");

        RuneSequence combined;
        combined.append(greeting);
        combined.append(target);
        combined.append(punctuation);

        std::cout << "  🔹 Combined result: \"" << combined.to_utf8() << "\"" << std::endl;
        std::cout << "  🔹 Total length: " << combined.size() << " runes" << std::endl;

        std::cout << std::endl;

        // === 6. 다국어 처리 ===
        std::cout << "🌍 6. Multilingual text processing:" << std::endl;

        auto multilingual = RuneSequence::from_utf8("English 한국어 日本語 中文 العربية");
        std::cout << "  🔹 Multilingual text: \"" << multilingual.to_utf8() << "\"" << std::endl;
        std::cout << "  🔹 Character count: " << multilingual.size() << std::endl;

        // 언어별 분석
        std::cout << "  🔹 Language analysis:" << std::endl;
        std::cout << "    - Primary language: " << static_cast<int>(multilingual.primary_language()) << std::endl;
        std::cout << "    - Contains multiple scripts: " <<
                     (multilingual.size() > 20 ? "Yes" : "No") << std::endl;

        std::cout << std::endl;

        // === 7. 비교 및 검증 ===
        std::cout << "⚖️ 7. Comparison and validation:" << std::endl;

        auto text1 = RuneSequence::from_utf8("Hello");
        auto text2 = RuneSequence::from_utf8("Hello");
        auto text3 = RuneSequence::from_utf8("World");

        std::cout << "  🔹 \"Hello\" == \"Hello\": " << (text1 == text2 ? "✅ Yes" : "❌ No") << std::endl;
        std::cout << "  🔹 \"Hello\" == \"World\": " << (text1 == text3 ? "✅ Yes" : "❌ No") << std::endl;
        std::cout << "  🔹 Empty check: " << (empty_seq.empty() ? "✅ Empty" : "❌ Not empty") << std::endl;

        std::cout << std::endl;

        // === 8. 실용적인 예제 ===
        std::cout << "💡 8. Practical example - Text statistics:" << std::endl;

        auto sample_text = RuneSequence::from_utf8("Hello, 안녕하세요! How are you? 어떻게 지내세요? 🌟");
        std::cout << "  🔹 Sample text: \"" << sample_text.to_utf8() << "\"" << std::endl;

        size_t letter_count = 0;
        size_t whitespace_count = 0;
        size_t punctuation_count = 0;

        for (const auto& rune : sample_text) {
            if (rune.is_letter()) {
                letter_count++;
            } else if (rune.is_whitespace()) {
                whitespace_count++;
            } else {
                punctuation_count++;
            }
        }

        std::cout << "  📊 Statistics:" << std::endl;
        std::cout << "    - Total characters: " << sample_text.size() << std::endl;
        std::cout << "    - Letters: " << letter_count << std::endl;
        std::cout << "    - Whitespace: " << whitespace_count << std::endl;
        std::cout << "    - Other (punct/symbols): " << punctuation_count << std::endl;

        std::cout << std::endl;
        std::cout << "🎉 === RuneSequence example completed successfully! ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        std::cerr << "💡 Tip: Ensure proper UTF-8 encoding and valid operations" << std::endl;
        return 1;
    }

    return 0;
}
