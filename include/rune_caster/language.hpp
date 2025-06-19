#pragma once

#include <cstdint>
#include <string_view>

/**
 * @file language.hpp
 * @brief Language identification and localization support
 *
 * This header provides language identification and localization
 * functionality for the Rune Caster framework.
 */

namespace rune_caster {

/**
 * @namespace rune_caster::language
 * @brief Language detection and identification functionality
 *
 * This namespace contains language codes, detection algorithms,
 * and utilities for working with multilingual text.
 */
namespace language {

/**
 * @brief Enumeration of supported language codes
 *
 * Represents the languages supported by the Rune Caster framework.
 * Each language is mapped to a unique identifier that can be used
 * for language-specific processing and detection.
 *
 * The enumeration follows ISO 639-1 language codes where applicable.
 *
 * @code
 * using language::Code;
 *
 * auto korean_text = RuneString::from_utf8("안녕하세요");
 * if (korean_text.primary_language() == Code::Korean) {
 *     std::cout << "Korean text detected!" << std::endl;
 * }
 * @endcode
 */
enum class Code : uint16_t {
    Unknown = 0,    ///< Unknown or undetected language
    Korean,         ///< 한국어 (ko-KR) - Korean
    English,        ///< English (en-US) - English
    Japanese,       ///< 日本語 (ja-JP) - Japanese
    Chinese,        ///< 中文 (zh-CN) - Chinese (Simplified)
    Arabic,         ///< العربية (ar) - Arabic
    Russian,        ///< Русский (ru) - Russian
    Spanish,        ///< Español (es) - Spanish
    French,         ///< Français (fr) - French
    German,         ///< Deutsch (de) - German
    Italian,        ///< Italiano (it) - Italian
    Portuguese,     ///< Português (pt) - Portuguese
    Dutch,          ///< Nederlands (nl) - Dutch
    Swedish,        ///< Svenska (sv) - Swedish
    Norwegian,      ///< Norsk (no) - Norwegian
    Danish,         ///< Dansk (da) - Danish
    Finnish,        ///< Suomi (fi) - Finnish
    Polish,         ///< Polski (pl) - Polish
    Czech,          ///< Čeština (cs) - Czech
    Hungarian,      ///< Magyar (hu) - Hungarian
    Turkish,        ///< Türkçe (tr) - Turkish
    Hebrew,         ///< עברית (he) - Hebrew
    Thai,           ///< ไทย (th) - Thai
    Vietnamese,     ///< Tiếng Việt (vi) - Vietnamese
    Indonesian,     ///< Bahasa Indonesia (id) - Indonesian
    Malay,          ///< Bahasa Melayu (ms) - Malay
    Hindi,          ///< हिन्दी (hi) - Hindi
    Bengali,        ///< বাংলা (bn) - Bengali
    Tamil,          ///< தமிழ் (ta) - Tamil
    Telugu,         ///< తెలుగు (te) - Telugu
    Gujarati,       ///< ગુજરાતી (gu) - Gujarati
    Marathi,        ///< मराठी (mr) - Marathi
    Punjabi,        ///< ਪੰਜਾਬੀ (pa) - Punjabi
    Urdu,           ///< اردو (ur) - Urdu
    Persian,        ///< فارسی (fa) - Persian/Farsi
    Pashto,         ///< پښتو (ps) - Pashto
    Kurdish,        ///< کوردی (ku) - Kurdish
    Max             ///< Sentinel value for enumeration bounds
};

/**
 * @brief Language detection result with confidence metrics
 *
 * Contains the result of language detection analysis, including
 * the detected language, confidence score, and character count.
 *
 * @code
 * DetectionResult result = detect_language(text);
 * if (result.confidence > 0.8) {
 *     std::cout << "High confidence detection: "
 *               << to_string(result.language) << std::endl;
 * }
 * @endcode
 */
struct DetectionResult {
    Code language;      ///< Detected language code
    double confidence;  ///< Confidence score (0.0 to 1.0)
    size_t char_count;  ///< Number of characters analyzed

    /**
     * @brief Construct a detection result
     * @param lang Detected language (default: Unknown)
     * @param conf Confidence score (default: 0.0)
     * @param count Character count (default: 0)
     */
    constexpr DetectionResult(Code lang = Code::Unknown,
                             double conf = 0.0,
                             size_t count = 0) noexcept
        : language(lang), confidence(conf), char_count(count) {}
};

/**
 * @brief Convert language code to ISO 639-1 string representation
 *
 * @param code Language code to convert
 * @return ISO language code string (e.g., "ko-KR", "en-US")
 * @note This function is constexpr and can be evaluated at compile time
 *
 * @code
 * auto lang_str = to_string(Code::Korean);  // Returns "ko-KR"
 * std::cout << "Language: " << lang_str << std::endl;
 * @endcode
 */
constexpr std::string_view to_string(Code code) noexcept {
    switch (code) {
        case Code::Korean: return "ko-KR";
        case Code::English: return "en-US";
        case Code::Japanese: return "ja-JP";
        case Code::Chinese: return "zh-CN";
        case Code::Arabic: return "ar";
        case Code::Russian: return "ru";
        case Code::Spanish: return "es";
        case Code::French: return "fr";
        case Code::German: return "de";
        case Code::Italian: return "it";
        case Code::Portuguese: return "pt";
        case Code::Dutch: return "nl";
        case Code::Swedish: return "sv";
        case Code::Norwegian: return "no";
        case Code::Danish: return "da";
        case Code::Finnish: return "fi";
        case Code::Polish: return "pl";
        case Code::Czech: return "cs";
        case Code::Hungarian: return "hu";
        case Code::Turkish: return "tr";
        case Code::Hebrew: return "he";
        case Code::Thai: return "th";
        case Code::Vietnamese: return "vi";
        case Code::Indonesian: return "id";
        case Code::Malay: return "ms";
        case Code::Hindi: return "hi";
        case Code::Bengali: return "bn";
        case Code::Tamil: return "ta";
        case Code::Telugu: return "te";
        case Code::Gujarati: return "gu";
        case Code::Marathi: return "mr";
        case Code::Punjabi: return "pa";
        case Code::Urdu: return "ur";
        case Code::Persian: return "fa";
        case Code::Pashto: return "ps";
        case Code::Kurdish: return "ku";
        default: return "unknown";
    }
}

/**
 * @brief Get the human-readable name of a language
 *
 * @param code Language code to get the name for
 * @return English name of the language (e.g., "Korean", "English")
 * @note This function is constexpr and can be evaluated at compile time
 *
 * @code
 * auto name = get_name(Code::Japanese);  // Returns "Japanese"
 * std::cout << "Language name: " << name << std::endl;
 * @endcode
 */
constexpr std::string_view get_name(Code code) noexcept {
    switch (code) {
        case Code::Korean: return "Korean";
        case Code::English: return "English";
        case Code::Japanese: return "Japanese";
        case Code::Chinese: return "Chinese";
        case Code::Arabic: return "Arabic";
        case Code::Russian: return "Russian";
        case Code::Spanish: return "Spanish";
        case Code::French: return "French";
        case Code::German: return "German";
        case Code::Italian: return "Italian";
        case Code::Portuguese: return "Portuguese";
        case Code::Dutch: return "Dutch";
        case Code::Swedish: return "Swedish";
        case Code::Norwegian: return "Norwegian";
        case Code::Danish: return "Danish";
        case Code::Finnish: return "Finnish";
        case Code::Polish: return "Polish";
        case Code::Czech: return "Czech";
        case Code::Hungarian: return "Hungarian";
        case Code::Turkish: return "Turkish";
        case Code::Hebrew: return "Hebrew";
        case Code::Thai: return "Thai";
        case Code::Vietnamese: return "Vietnamese";
        case Code::Indonesian: return "Indonesian";
        case Code::Malay: return "Malay";
        case Code::Hindi: return "Hindi";
        case Code::Bengali: return "Bengali";
        case Code::Tamil: return "Tamil";
        case Code::Telugu: return "Telugu";
        case Code::Gujarati: return "Gujarati";
        case Code::Marathi: return "Marathi";
        case Code::Punjabi: return "Punjabi";
        case Code::Urdu: return "Urdu";
        case Code::Persian: return "Persian";
        case Code::Pashto: return "Pashto";
        case Code::Kurdish: return "Kurdish";
        default: return "Unknown";
    }
}

} // namespace language
} // namespace rune_caster
