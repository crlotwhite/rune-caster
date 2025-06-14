#include "rune_caster/spell.hpp"
#include "rune_caster/unicode.hpp"
#include <string>

// Unicode 케이스 변환 라이브러리 (조건부 포함)
#ifdef RUNE_CASTER_HAS_UNI_ALGO
    #include <uni_algo/case.h>
#elif defined(RUNE_CASTER_HAS_ICU)
    #include <unicode/uchar.h>
    #include <unicode/unistr.h>
#endif

namespace rune_caster {
namespace spell {
namespace core {

CaseConverter::CaseConverter(CaseType case_type)
    : case_type_(case_type) {
}

RuneSequence CaseConverter::operator()(const RuneSequence& input) const {
    if (input.empty()) {
        return RuneSequence();
    }

    // Convert to UTF-8 for case conversion
    std::string utf8_text = input.to_utf8();
    std::string converted;

    try {
#ifdef RUNE_CASTER_HAS_UNI_ALGO
        // Use uni-algo for case conversion
        switch (case_type_) {
            case CaseType::Lower:
                converted = una::cases::to_lowercase_utf8(utf8_text);
                break;
            case CaseType::Upper:
                converted = una::cases::to_uppercase_utf8(utf8_text);
                break;
            case CaseType::Title:
                converted = una::cases::to_titlecase_utf8(utf8_text);
                break;
            default:
                converted = utf8_text;
                break;
        }
#elif defined(RUNE_CASTER_HAS_ICU)
        // Use ICU for case conversion (placeholder implementation)
        // TODO: Implement ICU case conversion
        converted = utf8_text;
#else
        // Use fallback ASCII-only implementation
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
        // Fallback implementation for all error cases
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