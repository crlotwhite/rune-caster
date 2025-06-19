#include "rune_caster/spell.hpp"
#include "rune_caster/unicode.hpp"
#include <string>

#ifdef RUNE_CASTER_HAS_UTFCPP
    #include <utf8.h>
    #include <cwctype>
#elif defined(RUNE_CASTER_HAS_ICU)
    #include <unicode/uchar.h>
    #include <unicode/unistr.h>
#endif

namespace rune_caster {
namespace spell {
namespace core {

CaseConverter::CaseConverter(CaseType case_type)
    : case_type_(case_type) {}

RuneSequence CaseConverter::operator()(const RuneSequence& input) const {
    if (input.empty()) {
        return RuneSequence();
    }

    std::string utf8_text = input.to_utf8();
    std::string converted;

    try {
#ifdef RUNE_CASTER_HAS_UTFCPP
        std::u32string utf32;
        utf8::utf8to32(utf8_text.begin(), utf8_text.end(), std::back_inserter(utf32));
        bool at_word_start = true;
        for (char32_t cp : utf32) {
            char32_t out = cp;
            switch (case_type_) {
                case CaseType::Lower:
                    out = static_cast<char32_t>(std::towlower(static_cast<wint_t>(cp)));
                    break;
                case CaseType::Upper:
                    out = static_cast<char32_t>(std::towupper(static_cast<wint_t>(cp)));
                    break;
                case CaseType::Title:
                    if (at_word_start)
                        out = static_cast<char32_t>(std::towupper(static_cast<wint_t>(cp)));
                    else
                        out = static_cast<char32_t>(std::towlower(static_cast<wint_t>(cp)));
                    at_word_start = unicode::is_whitespace(cp);
                    break;
                default:
                    break;
            }
            utf8::append(out, std::back_inserter(converted));
        }
#elif defined(RUNE_CASTER_HAS_ICU)
        // Use ICU for case conversion (placeholder implementation)
        converted = utf8_text;
#else
        converted.reserve(utf8_text.size());
        bool at_word_start = true;

        for (char ch : utf8_text) {
            switch (case_type_) {
                case CaseType::Lower:
                    if (ch >= 'A' && ch <= 'Z') {
                        converted.push_back(ch + 32);
                    } else {
                        converted.push_back(ch);
                    }
                    break;
                case CaseType::Upper:
                    if (ch >= 'a' && ch <= 'z') {
                        converted.push_back(ch - 32);
                    } else {
                        converted.push_back(ch);
                    }
                    break;
                case CaseType::Title:
                    if (ch == ' ' || ch == '\t' || ch == '\n') {
                        at_word_start = true;
                        converted.push_back(ch);
                    } else if (at_word_start && ch >= 'a' && ch <= 'z') {
                        converted.push_back(ch - 32);
                        at_word_start = false;
                    } else {
                        if (ch >= 'A' && ch <= 'Z' && !at_word_start) {
                            converted.push_back(ch + 32);
                        } else {
                            converted.push_back(ch);
                        }
                        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
                            at_word_start = false;
                        }
                    }
                    break;
            }
        }
#endif
    } catch (...) {
        converted = utf8_text;
    }

    return RuneSequence::from_utf8(converted);
}

std::string CaseConverter::description() const {
    switch (case_type_) {
        case CaseType::Lower:
            return "Lowercase Conversion";
        case CaseType::Upper:
            return "Uppercase Conversion";
        case CaseType::Title:
            return "Titlecase Conversion";
        default:
            return "Case Conversion";
    }
}

} // namespace core
} // namespace spell
} // namespace rune_caster
