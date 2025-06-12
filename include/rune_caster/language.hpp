#pragma once

#include <cstdint>
#include <string_view>

namespace rune_caster {
namespace language {

// 언어 코드
enum class Code : uint16_t {
    Unknown = 0,
    Korean,     // ko-KR
    English,    // en-US
    Japanese,   // ja-JP
    Chinese,    // zh-CN
    Arabic,     // ar
    Russian,    // ru
    Spanish,    // es
    French,     // fr
    German,     // de
    Italian,    // it
    Portuguese, // pt
    Dutch,      // nl
    Swedish,    // sv
    Norwegian,  // no
    Danish,     // da
    Finnish,    // fi
    Polish,     // pl
    Czech,      // cs
    Hungarian,  // hu
    Turkish,    // tr
    Hebrew,     // he
    Thai,       // th
    Vietnamese, // vi
    Indonesian, // id
    Malay,      // ms
    Hindi,      // hi
    Bengali,    // bn
    Tamil,      // ta
    Telugu,     // te
    Gujarati,   // gu
    Marathi,    // mr
    Punjabi,    // pa
    Urdu,       // ur
    Persian,    // fa
    Pashto,     // ps
    Kurdish,    // ku
    Max
};

// 언어 감지 결과
struct DetectionResult {
    Code language;
    double confidence;
    size_t char_count;

    constexpr DetectionResult(Code lang = Code::Unknown,
                             double conf = 0.0,
                             size_t count = 0) noexcept
        : language(lang), confidence(conf), char_count(count) {}
};

// 언어 코드를 문자열로 변환
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

// 언어 이름을 가져오기
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