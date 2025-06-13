/**
 * @file basic_usage.cpp
 * @brief Rune Caster 라이브러리의 기본 사용법을 시연하는 예제
 *
 * 이 예제는 다음 기능들을 다룹니다:
 * - Rune 객체 생성 및 다양한 언어 지원
 * - 문자 분류 및 속성 확인
 * - UTF-8 변환 및 문자 인코딩
 * - Factory 메서드 활용
 * - 음성학적 표현 설정
 * - 객체 비교 연산
 *
 * @author Rune-Caster Development Team
 * @since 1.0.0
 */

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
        // === 1. 기본 Rune 생성 ===
        std::cout << "🔤 1. Creating basic Runes:" << std::endl;

        // 영어 문자 - ASCII 범위
        Rune latin_a(U'A');
        std::cout << "  📝 Latin 'A': codepoint=" << static_cast<uint32_t>(latin_a.codepoint())
                  << ", language=" << static_cast<int>(latin_a.language()) << std::endl;

        // 한글 문자 - Unicode 범위 (완성형 한글)
        Rune hangul_ga(U'가');
        std::cout << "  🇰🇷 Hangul '가': codepoint=" << static_cast<uint32_t>(hangul_ga.codepoint())
                  << ", language=" << static_cast<int>(hangul_ga.language()) << std::endl;

        // 일본어 히라가나 - 일본어 문자 체계
        Rune hiragana_a(U'あ');
        std::cout << "  🇯🇵 Hiragana 'あ': codepoint=" << static_cast<uint32_t>(hiragana_a.codepoint())
                  << ", language=" << static_cast<int>(hiragana_a.language()) << std::endl;

        std::cout << std::endl;

        // === 2. 문자 분류 테스트 ===
        std::cout << "🔍 2. Character classification:" << std::endl;

        std::cout << "  📊 'A' Properties:" << std::endl;
        std::cout << "    - is_letter: " << (latin_a.is_letter() ? "✅" : "❌") << std::endl;
        std::cout << "    - is_vowel: " << (latin_a.is_vowel() ? "✅" : "❌") << std::endl;
        std::cout << "    - is_consonant: " << (latin_a.is_consonant() ? "✅" : "❌") << std::endl;

        Rune space(U' ');
        std::cout << "  📊 ' ' (space) Properties:" << std::endl;
        std::cout << "    - is_whitespace: " << (space.is_whitespace() ? "✅" : "❌") << std::endl;

        Rune digit(U'5');
        std::cout << "  📊 '5' Properties:" << std::endl;
        std::cout << "    - is_digit: " << (digit.is_digit() ? "✅" : "❌") << std::endl;

        std::cout << std::endl;

        // === 3. UTF-8 변환 ===
        std::cout << "🔄 3. UTF-8 conversion:" << std::endl;
        std::cout << "  💡 문자를 UTF-8 바이트 시퀀스로 변환합니다" << std::endl;

        std::cout << "  📤 'A' -> UTF-8: '" << latin_a.to_utf8() << "'" << std::endl;
        std::cout << "  📤 '가' -> UTF-8: '" << hangul_ga.to_utf8() << "'" << std::endl;
        std::cout << "  📤 'あ' -> UTF-8: '" << hiragana_a.to_utf8() << "'" << std::endl;

        std::cout << std::endl;

        // === 4. Factory 메서드 테스트 ===
        std::cout << "🏭 4. Factory methods:" << std::endl;
        std::cout << "  💡 문자열이나 특정 언어 힌트로 Rune 객체를 생성합니다" << std::endl;

        Rune from_utf8 = Rune::from_utf8("한");
        std::cout << "  🔨 From UTF-8 '한': codepoint=" << static_cast<uint32_t>(from_utf8.codepoint())
                  << ", language=" << static_cast<int>(from_utf8.language()) << std::endl;

        Rune with_language = Rune::from_utf8("A", language::Code::English);
        std::cout << "  🔨 'A' with English hint: language="
                  << static_cast<int>(with_language.language()) << std::endl;

        std::cout << std::endl;

        // === 5. 음성학적 표현 설정 ===
        std::cout << "🔊 5. Phonetic representation:" << std::endl;
        std::cout << "  💡 문자에 발음 정보를 추가할 수 있습니다" << std::endl;

        Rune phonetic_rune(U'가', language::Code::Korean, "ga");
        std::cout << "  🎵 '가' with phoneme 'ga': " << phonetic_rune.phoneme() << std::endl;

        phonetic_rune.set_phoneme("ka");
        std::cout << "  🎵 After setting to 'ka': " << phonetic_rune.phoneme() << std::endl;

        std::cout << std::endl;

        // === 6. 비교 연산 ===
        std::cout << "⚖️ 6. Comparison operations:" << std::endl;
        std::cout << "  💡 Rune 객체들 간의 동등성과 순서 비교가 가능합니다" << std::endl;

        Rune a1(U'A');
        Rune a2(U'A');
        Rune b(U'B');

        std::cout << "  📊 'A' == 'A': " << (a1 == a2 ? "✅ true" : "❌ false") << std::endl;
        std::cout << "  📊 'A' == 'B': " << (a1 == b ? "✅ true" : "❌ false") << std::endl;
        std::cout << "  📊 'A' < 'B': " << (a1 < b ? "✅ true" : "❌ false") << std::endl;

        std::cout << std::endl;

        // === 7. 추가 정보 ===
        std::cout << "ℹ️ 7. Additional Information:" << std::endl;
        std::cout << "  📝 Language codes are enumerated values representing different writing systems" << std::endl;
        std::cout << "  🌍 Supported languages include: English, Korean, Japanese, Chinese, etc." << std::endl;
        std::cout << "  🔤 Phonemic representation helps with pronunciation and linguistic analysis" << std::endl;
        std::cout << "  🚀 All operations are Unicode-compliant and support international text" << std::endl;

        std::cout << std::endl;
        std::cout << "🎉 === Example completed successfully! ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        std::cerr << "💡 Tip: Ensure proper UTF-8 encoding and valid Unicode codepoints" << std::endl;
        return 1;
    }

    return 0;
}