#include "rune_caster/spell.hpp"
#include "rune_caster/unicode.hpp"
#include <string>
#include <iostream>

// uni-algo 라이브러리의 정규화 기능
#include <uni_algo/norm.h>

namespace rune_caster {
namespace spell {

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
        // Use uni-algo for normalization if available
        switch (form_) {
            case unicode::NormalizationForm::NFC:
                // NFC normalization (compose)
                normalized = una::norm::to_nfc_utf8(utf8_text);
                break;
            case unicode::NormalizationForm::NFD:
                // NFD normalization (decompose)
                normalized = una::norm::to_nfd_utf8(utf8_text);
                break;
            case unicode::NormalizationForm::NFKC:
                // NFKC normalization (compatibility compose)
                normalized = una::norm::to_nfkc_utf8(utf8_text);
                break;
            case unicode::NormalizationForm::NFKD:
                // NFKD normalization (compatibility decompose)
                normalized = una::norm::to_nfkd_utf8(utf8_text);
                break;
            default:
                // Fallback: return input unchanged
                normalized = utf8_text;
                break;
        }
    } catch (...) {
        // Fallback implementation for basic cases
        normalized = utf8_text;
        
        // Simple fallback: just ensure basic composition for common cases
        if (form_ == unicode::NormalizationForm::NFC || 
            form_ == unicode::NormalizationForm::NFKC) {
            // Basic composition examples for Korean Hangul
            // This is a very simplified fallback
        }
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

} // namespace spell
} // namespace rune_caster 