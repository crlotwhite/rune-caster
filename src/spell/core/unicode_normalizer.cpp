#include "rune_caster/spell.hpp"
#include "rune_caster/unicode.hpp"
#include <string>
#include <iostream>

// Unicode 정규화 라이브러리 (조건부 포함)
#ifdef RUNE_CASTER_HAS_UNI_ALGO
    #include <uni_algo/norm.h>
#elif defined(RUNE_CASTER_HAS_ICU)
    #include <unicode/normalizer2.h>
    #include <unicode/unistr.h>
#endif

namespace rune_caster {
namespace spell {
namespace core {

UnicodeNormalizer::UnicodeNormalizer(unicode::NormalizationForm form)
    : form_(form) {
}

RuneSequence UnicodeNormalizer::operator()(const RuneSequence& input) const {
    if (input.empty()) {
        return RuneSequence();
    }

    // Convert to UTF-8 for normalization
    std::string utf8_text = input.to_utf8();
    std::string normalized;

    try {
#ifdef RUNE_CASTER_HAS_UNI_ALGO
        // Use uni-algo for normalization
        switch (form_) {
            case unicode::NormalizationForm::NFC:
                normalized = una::norm::to_nfc_utf8(utf8_text);
                break;
            case unicode::NormalizationForm::NFD:
                normalized = una::norm::to_nfd_utf8(utf8_text);
                break;
            case unicode::NormalizationForm::NFKC:
                normalized = una::norm::to_nfkc_utf8(utf8_text);
                break;
            case unicode::NormalizationForm::NFKD:
                normalized = una::norm::to_nfkd_utf8(utf8_text);
                break;
            default:
                normalized = utf8_text;
                break;
        }
#elif defined(RUNE_CASTER_HAS_ICU)
        // Use ICU for normalization (placeholder implementation)
        // TODO: Implement ICU normalization
        normalized = utf8_text;
#else
        // Fallback: no normalization library available
        normalized = utf8_text;
        
        // Simple fallback for basic composition (placeholder)
        if (form_ == unicode::NormalizationForm::NFC ||
            form_ == unicode::NormalizationForm::NFKC) {
            // Basic composition examples for Korean Hangul
            // This is a very simplified fallback
        }
#endif
    } catch (...) {
        // Fallback implementation for all error cases
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
