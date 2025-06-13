#include "rune_caster/rune_sequence.hpp"
#include <algorithm>
#include <stdexcept>
#include <map>

namespace rune_caster {

// === Constructors ===

RuneString::RuneString(language::Code primary_lang) noexcept
    : runes_()
    , primary_language_(primary_lang)
{
}

RuneString::RuneString(std::string_view utf8)
    : RuneString(from_utf8(utf8))
{
}

RuneString::RuneString(std::u16string_view utf16)
    : RuneString(from_utf16(utf16))
{
}

RuneString::RuneString(std::u32string_view utf32)
    : RuneString(from_utf32(utf32))
{
}

RuneString::RuneString(std::initializer_list<Rune> runes)
    : runes_(runes)
    , primary_language_(language::Code::Unknown)
{
    // Auto-detect primary language from the most frequent language in the sequence
    if (!runes_.empty()) {
        std::map<language::Code, size_t> language_counts;
        for (const auto& rune : runes_) {
            if (rune.language() != language::Code::Unknown) {
                ++language_counts[rune.language()];
            }
        }

        if (!language_counts.empty()) {
            primary_language_ = std::max_element(language_counts.begin(), language_counts.end(),
                [](const auto& a, const auto& b) { return a.second < b.second; })->first;
        }
    }
}

// === Conversion methods ===

std::string RuneString::to_utf8() const {
    std::string result;
    result.reserve(size() * 2); // Rough estimate for UTF-8 size

    for (const auto& rune : runes_) {
        result += rune.to_utf8();
    }

    return result;
}

std::u16string RuneString::to_utf16() const {
    std::u16string result;
    result.reserve(size() * 2); // Rough estimate for UTF-16 size

    for (const auto& rune : runes_) {
        auto utf16_char = rune.to_utf16();
        result += utf16_char;
    }

    return result;
}

std::u32string RuneString::to_utf32() const {
    std::u32string result;
    result.reserve(size());

    for (const auto& rune : runes_) {
        result.push_back(rune.codepoint());
    }

    return result;
}

// === String operations ===

RuneString& RuneString::append(const RuneString& other) {
    runes_.insert(runes_.end(), other.runes_.begin(), other.runes_.end());
    return *this;
}

RuneString& RuneString::append(const Rune& rune) {
    runes_.push_back(rune);
    return *this;
}

RuneString& RuneString::append(std::string_view utf8) {
    auto other = from_utf8(utf8);
    return append(other);
}

RuneString RuneString::substr(size_type start, size_type length) const {
    if (start >= size()) {
        return RuneString(primary_language_);
    }

    size_type actual_length = (length == npos) ? (size() - start) : std::min(length, size() - start);

    RuneString result(primary_language_);
    result.runes_.assign(runes_.begin() + start, runes_.begin() + start + actual_length);

    return result;
}

// === Search operations ===

RuneString::size_type RuneString::find(const Rune& rune, size_type pos) const noexcept {
    if (pos >= size()) {
        return npos;
    }

    auto it = std::find(runes_.begin() + pos, runes_.end(), rune);
    return (it != runes_.end()) ? std::distance(runes_.begin(), it) : npos;
}

RuneString::size_type RuneString::find(const RuneString& str, size_type pos) const noexcept {
    if (str.empty()) {
        return pos;
    }

    if (pos >= size() || str.size() > size() - pos) {
        return npos;
    }

    auto it = std::search(runes_.begin() + pos, runes_.end(),
                         str.runes_.begin(), str.runes_.end());

    return (it != runes_.end()) ? std::distance(runes_.begin(), it) : npos;
}

bool RuneString::contains(const Rune& rune) const noexcept {
    return find(rune) != npos;
}

bool RuneString::contains(const RuneString& str) const noexcept {
    return find(str) != npos;
}

// === Factory methods ===

RuneString RuneString::from_utf8(std::string_view utf8_text) {
    RuneString result;

    if (utf8_text.empty()) {
        return result;
}

    // Reserve space (rough estimate)
    result.runes_.reserve(utf8_text.size());

    std::map<language::Code, size_t> language_counts;

    size_t i = 0;
    while (i < utf8_text.size()) {
        // Find the end of current UTF-8 character
        size_t char_length = 1;
        unsigned char first_byte = static_cast<unsigned char>(utf8_text[i]);

        if (first_byte <= 0x7F) {
            char_length = 1;
        } else if ((first_byte & 0xE0) == 0xC0) {
            char_length = 2;
        } else if ((first_byte & 0xF0) == 0xE0) {
            char_length = 3;
        } else if ((first_byte & 0xF8) == 0xF0) {
            char_length = 4;
        } else {
            // Invalid UTF-8, skip this byte
            ++i;
            continue;
        }

        if (i + char_length > utf8_text.size()) {
            break; // Incomplete character at end of string
        }

    try {
            std::string_view char_view = utf8_text.substr(i, char_length);
            Rune rune = Rune::from_utf8(char_view);
            result.runes_.push_back(rune);

            // Count languages for primary language detection
            if (rune.language() != language::Code::Unknown) {
                ++language_counts[rune.language()];
        }
        } catch (const std::invalid_argument&) {
            // Skip invalid UTF-8 sequences
        }

        i += char_length;
    }

    // Set primary language based on most frequent language
    if (!language_counts.empty()) {
        result.primary_language_ = std::max_element(language_counts.begin(), language_counts.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; })->first;
    }

    return result;
}

RuneString RuneString::from_utf8(std::string_view utf8_text, language::Code lang) {
    RuneString result = from_utf8(utf8_text);
    result.set_primary_language(lang);

    // Also set language hint for all runes if they don't have a specific language
    for (auto& rune : result.runes_) {
        if (rune.language() == language::Code::Unknown) {
            rune.set_language(lang);
        }
    }

    return result;
}

RuneString RuneString::from_utf16(std::u16string_view utf16_text) {
    RuneString result;

    if (utf16_text.empty()) {
        return result;
    }

    result.runes_.reserve(utf16_text.size());
    std::map<language::Code, size_t> language_counts;

    size_t i = 0;
    while (i < utf16_text.size()) {
        try {
            char16_t first = utf16_text[i];

            if (first >= 0xD800 && first <= 0xDBFF) {
                // High surrogate - need to get the low surrogate too
                if (i + 1 >= utf16_text.size()) {
                    break; // Incomplete surrogate pair
                }
                std::u16string_view char_view = utf16_text.substr(i, 2);
                Rune rune = Rune::from_utf16(char_view);
                result.runes_.push_back(rune);

                if (rune.language() != language::Code::Unknown) {
                    ++language_counts[rune.language()];
                }

                i += 2;
            } else if (first >= 0xDC00 && first <= 0xDFFF) {
                // Unexpected low surrogate - skip
                ++i;
            } else {
                // BMP character
                std::u16string_view char_view = utf16_text.substr(i, 1);
                Rune rune = Rune::from_utf16(char_view);
                result.runes_.push_back(rune);

                if (rune.language() != language::Code::Unknown) {
                    ++language_counts[rune.language()];
                }

                ++i;
            }
        } catch (const std::invalid_argument&) {
            // Skip invalid UTF-16 sequences
            ++i;
        }
    }

    // Set primary language
    if (!language_counts.empty()) {
        result.primary_language_ = std::max_element(language_counts.begin(), language_counts.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; })->first;
    }

    return result;
}

RuneString RuneString::from_utf32(std::u32string_view utf32_text) {
    RuneString result;

    if (utf32_text.empty()) {
        return result;
    }

    result.runes_.reserve(utf32_text.size());
    std::map<language::Code, size_t> language_counts;

    for (char32_t codepoint : utf32_text) {
        try {
            Rune rune(codepoint);
            result.runes_.push_back(rune);

            if (rune.language() != language::Code::Unknown) {
                ++language_counts[rune.language()];
            }
        } catch (const std::invalid_argument&) {
            // Skip invalid codepoints
        }
    }

    // Set primary language
    if (!language_counts.empty()) {
        result.primary_language_ = std::max_element(language_counts.begin(), language_counts.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; })->first;
    }

    return result;
}

} // namespace rune_caster