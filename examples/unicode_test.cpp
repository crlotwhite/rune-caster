#include <iostream>
#include <iomanip>
#include <string>
#include "rune_caster/rune.hpp"

using namespace rune_caster;

int main() {
    std::cout << "=== Unicode Debug Test ===" << std::endl;

    // 일본어 히라가나 'あ' 테스트
    Rune hiragana_a(U'あ');

    std::cout << "1. Hiragana 'あ' analysis:" << std::endl;
    std::cout << "  Codepoint: U+" << std::hex << std::uppercase
              << static_cast<uint32_t>(hiragana_a.codepoint()) << std::dec << std::endl;
    std::cout << "  Language: " << static_cast<int>(hiragana_a.language()) << std::endl;

    // UTF-8 바이트 분석
    std::string utf8_result = hiragana_a.to_utf8();
    std::cout << "  UTF-8 bytes: ";
    for (unsigned char byte : utf8_result) {
        std::cout << "0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                  << static_cast<unsigned int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;

    // 예상값: あ = U+3042 = UTF-8: 0xE3 0x81 0x82
    std::cout << "  Expected: 0xE3 0x81 0x82" << std::endl;
    std::cout << "  String length: " << utf8_result.length() << " bytes" << std::endl;

    std::cout << std::endl;

    // 한글도 테스트
    Rune hangul_ga(U'가');
    std::cout << "2. Hangul '가' analysis:" << std::endl;
    std::cout << "  Codepoint: U+" << std::hex << std::uppercase
              << static_cast<uint32_t>(hangul_ga.codepoint()) << std::dec << std::endl;

    std::string hangul_utf8 = hangul_ga.to_utf8();
    std::cout << "  UTF-8 bytes: ";
    for (unsigned char byte : hangul_utf8) {
        std::cout << "0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                  << static_cast<unsigned int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;

    // 예상값: 가 = U+AC00 = UTF-8: 0xEA 0xB0 0x80
    std::cout << "  Expected: 0xEA 0xB0 0x80" << std::endl;

    std::cout << std::endl;

    // 콘솔 출력 테스트
    std::cout << "3. Console output test:" << std::endl;
    std::cout << "  Direct UTF-8: " << utf8_result << std::endl;
    std::cout << "  Direct Hangul: " << hangul_utf8 << std::endl;

    // Windows 콘솔 확인
    std::cout << "4. Console info:" << std::endl;
    std::cout << "  If you see '?" "?' above, it's a console display issue, not Unicode processing." << std::endl;
    std::cout << "  The UTF-8 bytes are correct, but Windows console can't display them." << std::endl;
    std::cout << "  Try: Windows Terminal, VS Code terminal, or set console font to 'NSimSun'." << std::endl;

    return 0;
}