#include "rune_caster/rune_sequence.hpp"
#include <sstream>
#include <algorithm>
#include <codecvt>
#include <locale>

namespace rune_caster {

// === Constructors ===

RuneSequence::RuneSequence() noexcept
    : runes_()
    , primary_language_(language::Code::Unknown)
{
}

RuneSequence::RuneSequence(language::Code primary_lang) noexcept
    : runes_()
    , primary_language_(primary_lang)
{
}

// === Sequence-specific operations ===

std::string RuneSequence::to_utf8() const {
    std::string result;
    result.reserve(size() * 4); // rough estimate for UTF-8 encoding

    for (const auto& rune : runes_) {
        result += rune.to_utf8();
    }

    return result;
}

void RuneSequence::append(const RuneSequence& other) {
    runes_.reserve(size() + other.size());
    runes_.insert(runes_.end(), other.runes_.begin(), other.runes_.end());
}

RuneSequence RuneSequence::substr(size_type start, size_type length) const {
    if (start >= size()) {
        return RuneSequence(primary_language_);
    }

    const size_type actual_length = (length == std::string::npos)
        ? (size() - start)
        : std::min(length, size() - start);

    RuneSequence result(primary_language_);
    result.runes_.reserve(actual_length);

    auto start_it = runes_.begin() + static_cast<difference_type>(start);
    auto end_it = start_it + static_cast<difference_type>(actual_length);

    result.runes_.assign(start_it, end_it);

    return result;
}

// === Factory methods ===

RuneSequence RuneSequence::from_utf8(std::string_view utf8_text) {
    return from_utf8(utf8_text, language::Code::Unknown);
}

RuneSequence RuneSequence::from_utf8(std::string_view utf8_text, language::Code lang) {
    RuneSequence result(lang);

    // Simple UTF-8 to char32_t conversion
    // Note: This is a basic implementation; a production version would use ICU
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    try {
        std::u32string u32_text = converter.from_bytes(utf8_text.data(), utf8_text.data() + utf8_text.size());

        result.runes_.reserve(u32_text.size());

        for (char32_t cp : u32_text) {
            if (lang != language::Code::Unknown) {
                result.runes_.emplace_back(cp, lang);
            } else {
                result.runes_.emplace_back(cp);
            }
        }
    } catch (const std::exception& e) {
        // Fallback: create runes for individual bytes (assuming ASCII-compatible)
        for (char c : utf8_text) {
            if (static_cast<unsigned char>(c) < 128) { // ASCII
                if (lang != language::Code::Unknown) {
                    result.runes_.emplace_back(static_cast<char32_t>(c), lang);
                } else {
                    result.runes_.emplace_back(static_cast<char32_t>(c));
                }
            }
        }
    }

    return result;
}

} // namespace rune_caster