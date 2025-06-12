#include <iostream>
#include <string>
#include "rune_caster/rune.hpp"
#include "rune_caster/version.hpp"

using namespace rune_caster;

int main() {
    std::cout << "=== Rune Caster Basic Usage Example ===" << std::endl;
    std::cout << "Version: " << version_string << std::endl;
    std::cout << "Description: " << version_description << std::endl;
    std::cout << std::endl;

    try {
        // === 기본 Rune 생성 ===
        std::cout << "1. Creating basic Runes:" << std::endl;

        // 영어 문자
        Rune latin_a(U'A');
                std::cout << "  Latin 'A': codepoint=" << static_cast<uint32_t>(latin_a.codepoint())
                  << ", language=" << static_cast<int>(latin_a.language()) << std::endl;

        // 한글 문자
        Rune hangul_ga(U'가');
        std::cout << "  Hangul '가': codepoint=" << static_cast<uint32_t>(hangul_ga.codepoint())
                  << ", language=" << static_cast<int>(hangul_ga.language()) << std::endl;

        // 일본어 히라가나
        Rune hiragana_a(U'あ');
        std::cout << "  Hiragana 'あ': codepoint=" << static_cast<uint32_t>(hiragana_a.codepoint())
                  << ", language=" << static_cast<int>(hiragana_a.language()) << std::endl;

        std::cout << std::endl;

        // === 문자 분류 테스트 ===
        std::cout << "2. Character classification:" << std::endl;

        std::cout << "  'A' is_letter: " << latin_a.is_letter() << std::endl;
        std::cout << "  'A' is_vowel: " << latin_a.is_vowel() << std::endl;
        std::cout << "  'A' is_consonant: " << latin_a.is_consonant() << std::endl;

        Rune space(U' ');
        std::cout << "  ' ' is_whitespace: " << space.is_whitespace() << std::endl;

        Rune digit(U'5');
        std::cout << "  '5' is_digit: " << digit.is_digit() << std::endl;

        std::cout << std::endl;

        // === UTF-8 변환 ===
        std::cout << "3. UTF-8 conversion:" << std::endl;

        std::cout << "  'A' -> UTF-8: '" << latin_a.to_utf8() << "'" << std::endl;
        std::cout << "  '가' -> UTF-8: '" << hangul_ga.to_utf8() << "'" << std::endl;
        std::cout << "  'あ' -> UTF-8: '" << hiragana_a.to_utf8() << "'" << std::endl;

        std::cout << std::endl;

        // === Factory 메서드 테스트 ===
        std::cout << "4. Factory methods:" << std::endl;

        Rune from_utf8 = Rune::from_utf8("한");
        std::cout << "  From UTF-8 '한': codepoint=" << static_cast<uint32_t>(from_utf8.codepoint())
                  << ", language=" << static_cast<int>(from_utf8.language()) << std::endl;

        Rune with_language = Rune::from_utf8("A", language::Code::English);
        std::cout << "  'A' with English hint: language="
                  << static_cast<int>(with_language.language()) << std::endl;

        std::cout << std::endl;

        // === 음성학적 표현 설정 ===
        std::cout << "5. Phonetic representation:" << std::endl;

        Rune phonetic_rune(U'가', language::Code::Korean, "ga");
        std::cout << "  '가' with phoneme 'ga': " << phonetic_rune.phoneme() << std::endl;

        phonetic_rune.set_phoneme("ka");
        std::cout << "  After setting to 'ka': " << phonetic_rune.phoneme() << std::endl;

        std::cout << std::endl;

        // === 비교 연산 ===
        std::cout << "6. Comparison operations:" << std::endl;

        Rune a1(U'A');
        Rune a2(U'A');
        Rune b(U'B');

        std::cout << "  'A' == 'A': " << (a1 == a2) << std::endl;
        std::cout << "  'A' == 'B': " << (a1 == b) << std::endl;
        std::cout << "  'A' < 'B': " << (a1 < b) << std::endl;

        std::cout << std::endl;
        std::cout << "=== Example completed successfully! ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}