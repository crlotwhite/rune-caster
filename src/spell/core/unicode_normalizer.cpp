#include "rune_caster/spell.hpp"
#include "rune_caster/unicode.hpp"
#include <string>

#ifdef RUNE_CASTER_HAS_UTFCPP
    #include <utf8.h>
#elif defined(RUNE_CASTER_HAS_ICU)
    #include <unicode/normalizer2.h>
    #include <unicode/unistr.h>
#endif

namespace rune_caster {
namespace spell {
namespace core {

UnicodeNormalizer::UnicodeNormalizer(unicode::NormalizationForm form)
    : form_(form) {}

RuneSequence UnicodeNormalizer::operator()(const RuneSequence& input) const {
    if (input.empty()) {
        return RuneSequence();
    }

    std::string utf8_text = input.to_utf8();
    std::string normalized;

    try {
#ifdef RUNE_CASTER_HAS_UTFCPP
        std::u32string utf32;
        utf8::utf8to32(utf8_text.begin(), utf8_text.end(), std::back_inserter(utf32));
        normalized.clear();
        utf8::utf32to8(utf32.begin(), utf32.end(), std::back_inserter(normalized));
#elif defined(RUNE_CASTER_HAS_ICU)
        // Use ICU for normalization (placeholder implementation)
        normalized = utf8_text;
#else
        normalized = utf8_text;
#endif
    } catch (...) {
        normalized = utf8_text;
    }

    return RuneSequence::from_utf8(normalized);
}

std::string UnicodeNormalizer::description() const {
    switch (form_) {
        case unicode::NormalizationForm::NFC:
            return "Unicode NFC Normalization";
        case unicode::NormalizationForm::NFD:
            return "Unicode NFD Normalization";
        case unicode::NormalizationForm::NFKC:
            return "Unicode NFKC Normalization";
        case unicode::NormalizationForm::NFKD:
            return "Unicode NFKD Normalization";
        default:
            return "Unicode Normalization";
    }
}

} // namespace core
} // namespace spell
} // namespace rune_caster
