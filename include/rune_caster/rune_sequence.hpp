#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <iterator>
#include <ranges>
#include <span>

#include "rune.hpp"
#include "language.hpp"

namespace rune_caster {

/**
 * @brief A sequence container for Rune objects (API Design Document: RuneString)
 *
 * RuneString provides an STL-compatible container for storing and
 * manipulating sequences of Rune objects. It supports typical container
 * operations and provides factory methods for creating sequences from
 * text strings.
 *
 * This class follows the API Design Document specification for RuneString
 * with full C++20 features support including ranges and concepts.
 */
class RuneString {
public:
    // === STL-compatible typedefs ===
    using value_type = Rune;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = Rune&;
    using const_reference = const Rune&;
    using pointer = Rune*;
    using const_pointer = const Rune*;
    using iterator = std::vector<Rune>::iterator;
    using const_iterator = std::vector<Rune>::const_iterator;
    using reverse_iterator = std::vector<Rune>::reverse_iterator;
    using const_reverse_iterator = std::vector<Rune>::const_reverse_iterator;

    // === Special constants ===
    static constexpr size_type npos = std::string::npos;

private:
    std::vector<Rune> runes_;
    language::Code primary_language_;

public:
    // === Constructors (API Design Document enhanced) ===

    /**
     * @brief Default constructor
     */
    RuneString() noexcept = default;

    /**
     * @brief Construct with specific primary language
     * @param primary_lang The primary language of the sequence
     */
    explicit RuneString(language::Code primary_lang) noexcept;

    /**
     * @brief Construct from UTF-8 string view
     * @param utf8 UTF-8 encoded string
     */
    explicit RuneString(std::string_view utf8);

    /**
     * @brief Construct from UTF-16 string view
     * @param utf16 UTF-16 encoded string
     */
    explicit RuneString(std::u16string_view utf16);

    /**
     * @brief Construct from UTF-32 string view
     * @param utf32 UTF-32 encoded string
     */
    explicit RuneString(std::u32string_view utf32);

    /**
     * @brief Construct from initializer list
     * @param runes Initializer list of Rune objects
     */
    RuneString(std::initializer_list<Rune> runes);

    /**
     * @brief Construct from iterator range (C++20 concepts)
     * @tparam It Input iterator type
     * @param first Iterator to the beginning
     * @param last Iterator to the end
     */
    template<std::input_iterator It>
    requires std::convertible_to<std::iter_value_t<It>, Rune>
    RuneString(It first, It last);

    /**
     * @brief Construct from range (C++20 ranges)
     * @tparam R Range type
     * @param range Input range
     */
    template<std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, Rune>
    explicit RuneString(R&& range);

    // === Copy/Move semantics ===
    RuneString(const RuneString&) = default;
    RuneString(RuneString&&) noexcept = default;
    RuneString& operator=(const RuneString&) = default;
    RuneString& operator=(RuneString&&) noexcept = default;
    ~RuneString() = default;

    // === Iterator access ===

    iterator begin() noexcept { return runes_.begin(); }
    const_iterator begin() const noexcept { return runes_.begin(); }
    const_iterator cbegin() const noexcept { return runes_.cbegin(); }

    iterator end() noexcept { return runes_.end(); }
    const_iterator end() const noexcept { return runes_.end(); }
    const_iterator cend() const noexcept { return runes_.cend(); }

    reverse_iterator rbegin() noexcept { return runes_.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return runes_.rbegin(); }
    const_reverse_iterator crbegin() const noexcept { return runes_.crbegin(); }

    reverse_iterator rend() noexcept { return runes_.rend(); }
    const_reverse_iterator rend() const noexcept { return runes_.rend(); }
    const_reverse_iterator crend() const noexcept { return runes_.crend(); }

    // === Capacity ===

    [[nodiscard]] bool empty() const noexcept { return runes_.empty(); }
    [[nodiscard]] size_type size() const noexcept { return runes_.size(); }
    [[nodiscard]] size_type length() const noexcept { return size(); }
    [[nodiscard]] size_type max_size() const noexcept { return runes_.max_size(); }

    void reserve(size_type new_cap) { runes_.reserve(new_cap); }
    [[nodiscard]] size_type capacity() const noexcept { return runes_.capacity(); }
    void shrink_to_fit() { runes_.shrink_to_fit(); }

    // === Element access ===

    [[nodiscard]] reference at(size_type pos) { return runes_.at(pos); }
    [[nodiscard]] const_reference at(size_type pos) const { return runes_.at(pos); }

    [[nodiscard]] reference operator[](size_type pos) noexcept { return runes_[pos]; }
    [[nodiscard]] const_reference operator[](size_type pos) const noexcept { return runes_[pos]; }

    [[nodiscard]] reference front() { return runes_.front(); }
    [[nodiscard]] const_reference front() const { return runes_.front(); }

    [[nodiscard]] reference back() { return runes_.back(); }
    [[nodiscard]] const_reference back() const { return runes_.back(); }

    [[nodiscard]] pointer data() noexcept { return runes_.data(); }
    [[nodiscard]] const_pointer data() const noexcept { return runes_.data(); }

    // === Views ===

    /**
     * @brief Get a mutable span view of the underlying runes
     */
    [[nodiscard]] std::span<Rune> span() noexcept { return std::span<Rune>(runes_.data(), runes_.size()); }

    /**
     * @brief Get a read-only span view of the underlying runes
     */
    [[nodiscard]] std::span<const Rune> span() const noexcept { return std::span<const Rune>(runes_.data(), runes_.size()); }

    /**
     * @brief Get a sub-view (lazy slice) without copying
     * @param start Starting index
     * @param length Number of elements (default npos → until end)
     */
    [[nodiscard]] std::span<const Rune> slice_view(size_type start, size_type length = npos) const noexcept {
        if (start >= size()) return {};
        size_type actual = (length == npos || start + length > size()) ? size() - start : length;
        return std::span<const Rune>(data() + start, actual);
    }

    [[nodiscard]] std::span<Rune> slice_view(size_type start, size_type length = npos) noexcept {
        if (start >= size()) return {};
        size_type actual = (length == npos || start + length > size()) ? size() - start : length;
        return std::span<Rune>(data() + start, actual);
    }

    // === Implicit conversion to span ===
    operator std::span<Rune>() noexcept { return span(); }
    operator std::span<const Rune>() const noexcept { return span(); }

    // === Modifiers ===

    void clear() noexcept { runes_.clear(); }

    iterator insert(const_iterator pos, const Rune& rune) {
        return runes_.insert(pos, rune);
    }

    iterator insert(const_iterator pos, Rune&& rune) {
        return runes_.insert(pos, std::move(rune));
    }

    template<class... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        return runes_.emplace(pos, std::forward<Args>(args)...);
    }

    iterator erase(const_iterator pos) {
        return runes_.erase(pos);
    }

    iterator erase(const_iterator first, const_iterator last) {
        return runes_.erase(first, last);
    }

    void push_back(const Rune& rune) {
        runes_.push_back(rune);
    }

    void push_back(Rune&& rune) {
        runes_.push_back(std::move(rune));
    }

    template<class... Args>
    reference emplace_back(Args&&... args) {
        return runes_.emplace_back(std::forward<Args>(args)...);
    }

    void pop_back() {
        runes_.pop_back();
    }

    void resize(size_type count) {
        runes_.resize(count);
    }

    void resize(size_type count, const Rune& value) {
        runes_.resize(count, value);
    }

    void swap(RuneString& other) noexcept {
        runes_.swap(other.runes_);
        std::swap(primary_language_, other.primary_language_);
    }

    // === String operations (API Design Document requirement) ===

    /**
     * @brief Get the primary language of the sequence
     * @return The primary language code
     */
    [[nodiscard]] language::Code primary_language() const noexcept {
        return primary_language_;
    }

    /**
     * @brief Set the primary language of the sequence
     * @param lang The new primary language
     */
    void set_primary_language(language::Code lang) noexcept {
        primary_language_ = lang;
    }

    /**
     * @brief Convert the sequence to UTF-8 string
     * @return UTF-8 encoded string
     */
    [[nodiscard]] std::string to_utf8() const;

    /**
     * @brief Convert the sequence to UTF-16 string
     * @return UTF-16 encoded string
     */
    [[nodiscard]] std::u16string to_utf16() const;

    /**
     * @brief Convert the sequence to UTF-32 string
     * @return UTF-32 encoded string
     */
    [[nodiscard]] std::u32string to_utf32() const;

    /**
     * @brief Append another RuneString
     * @param other The RuneString to append
     */
    RuneString& append(const RuneString& other);

    /**
     * @brief Append a single Rune
     * @param rune The Rune to append
     */
    RuneString& append(const Rune& rune);

    /**
     * @brief Append a UTF-8 string
     * @param utf8 The UTF-8 string to append
     */
    RuneString& append(std::string_view utf8);

    /**
     * @brief Create a substring
     * @param start Starting position
     * @param length Number of characters (default: all remaining)
     * @return New RuneString containing the substring
     */
    [[nodiscard]] RuneString substr(size_type start, size_type length = npos) const;

    // === Search operations (API Design Document requirement) ===

    /**
     * @brief Find first occurrence of a Rune
     * @param rune The Rune to search for
     * @param pos Starting position
     * @return Position of first match, or npos if not found
     */
    [[nodiscard]] size_type find(const Rune& rune, size_type pos = 0) const noexcept;

    /**
     * @brief Find first occurrence of a RuneString
     * @param str The RuneString to search for
     * @param pos Starting position
     * @return Position of first match, or npos if not found
     */
    [[nodiscard]] size_type find(const RuneString& str, size_type pos = 0) const noexcept;

    /**
     * @brief Check if the string contains a Rune
     * @param rune The Rune to search for
     * @return true if found, false otherwise
     */
    [[nodiscard]] bool contains(const Rune& rune) const noexcept;

    /**
     * @brief Check if the string contains a substring
     * @param str The substring to search for
     * @return true if found, false otherwise
     */
    [[nodiscard]] bool contains(const RuneString& str) const noexcept;

    // === Factory methods (API Design Document requirement) ===

    /**
     * @brief Create a RuneString from UTF-8 text
     * @param utf8_text UTF-8 encoded text
     * @return RuneString object
     */
    static RuneString from_utf8(std::string_view utf8_text);

    /**
     * @brief Create a RuneString from UTF-8 text with language hint
     * @param utf8_text UTF-8 encoded text
     * @param lang Language hint for processing
     * @return RuneString object
     */
    static RuneString from_utf8(std::string_view utf8_text, language::Code lang);

    /**
     * @brief Create a RuneString from UTF-16 text
     * @param utf16_text UTF-16 encoded text
     * @return RuneString object
     */
    static RuneString from_utf16(std::u16string_view utf16_text);

    /**
     * @brief Create a RuneString from UTF-32 text
     * @param utf32_text UTF-32 encoded text
     * @return RuneString object
     */
    static RuneString from_utf32(std::u32string_view utf32_text);

    // === Comparison operators (C++20 three-way comparison) ===

    [[nodiscard]] auto operator<=>(const RuneString& other) const noexcept = default;
    [[nodiscard]] bool operator==(const RuneString& other) const noexcept = default;

    // === String concatenation operators ===

    RuneString& operator+=(const RuneString& other) { return append(other); }
    RuneString& operator+=(const Rune& rune) { return append(rune); }
    RuneString& operator+=(std::string_view utf8) { return append(utf8); }

    [[nodiscard]] friend RuneString operator+(const RuneString& lhs, const RuneString& rhs) {
        RuneString result = lhs;
        result += rhs;
        return result;
    }

    [[nodiscard]] friend RuneString operator+(const RuneString& lhs, const Rune& rhs) {
        RuneString result = lhs;
        result += rhs;
        return result;
    }

    [[nodiscard]] friend RuneString operator+(const Rune& lhs, const RuneString& rhs) {
        RuneString result;
        result.reserve(1 + rhs.size());
        result += lhs;
        result += rhs;
        return result;
    }
};

// === Template implementations ===

template<std::input_iterator It>
requires std::convertible_to<std::iter_value_t<It>, Rune>
RuneString::RuneString(It first, It last)
    : runes_(first, last)
    , primary_language_(language::Code::Unknown)
{
}

template<std::ranges::input_range R>
requires std::convertible_to<std::ranges::range_value_t<R>, Rune>
RuneString::RuneString(R&& range)
    : runes_(std::ranges::begin(range), std::ranges::end(range))
    , primary_language_(language::Code::Unknown)
{
}

// === Backward compatibility alias ===

/**
 * @brief Backward compatibility alias for RuneString
 *
 * This allows existing code using RuneSequence to continue working
 * while transitioning to the new RuneString API.
 */
using RuneSequence = RuneString;

// === Free functions ===

/**
 * @brief Swap two RuneString objects
 * @param lhs First RuneString
 * @param rhs Second RuneString
 */
inline void swap(RuneString& lhs, RuneString& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace rune_caster

// === STL interoperability helpers ===
#include <functional>
#include <ranges>

namespace std::ranges {
    template<>
    inline constexpr bool enable_borrowed_range<rune_caster::RuneString> = true;
}

namespace std {
    template<>
    struct hash<rune_caster::RuneString> {
        size_t operator()(const rune_caster::RuneString& rs) const noexcept {
            return std::hash<std::string>{}(rs.to_utf8());
        }
    };
}
