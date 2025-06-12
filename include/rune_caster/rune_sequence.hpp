#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <iterator>

#include "rune.hpp"
#include "language.hpp"

namespace rune_caster {

/**
 * @brief A sequence container for Rune objects
 *
 * RuneSequence provides an STL-compatible container for storing and
 * manipulating sequences of Rune objects. It supports typical container
 * operations and provides factory methods for creating sequences from
 * text strings.
 */
class RuneSequence {
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

private:
    std::vector<Rune> runes_;
    language::Code primary_language_;

public:
    // === Constructors ===

    /**
     * @brief Default constructor
     */
    RuneSequence() noexcept;

    /**
     * @brief Construct with specific primary language
     * @param primary_lang The primary language of the sequence
     */
    explicit RuneSequence(language::Code primary_lang) noexcept;

    /**
     * @brief Copy constructor
     */
    RuneSequence(const RuneSequence&) = default;

    /**
     * @brief Move constructor
     */
    RuneSequence(RuneSequence&&) noexcept = default;

    /**
     * @brief Copy assignment
     */
    RuneSequence& operator=(const RuneSequence&) = default;

    /**
     * @brief Move assignment
     */
    RuneSequence& operator=(RuneSequence&&) noexcept = default;

    /**
     * @brief Destructor
     */
    ~RuneSequence() = default;

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

    void swap(RuneSequence& other) noexcept {
        runes_.swap(other.runes_);
        std::swap(primary_language_, other.primary_language_);
    }

    // === Sequence-specific operations ===

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
     * @brief Append another sequence to this one
     * @param other The sequence to append
     */
    void append(const RuneSequence& other);

    /**
     * @brief Create a subsequence
     * @param start Start position
     * @param length Length of subsequence
     * @return New RuneSequence containing the subsequence
     */
    [[nodiscard]] RuneSequence substr(size_type start, size_type length = std::string::npos) const;

    // === Factory methods ===

    /**
     * @brief Create sequence from UTF-8 string
     * @param utf8_text UTF-8 encoded text
     * @return RuneSequence created from the text
     */
    static RuneSequence from_utf8(std::string_view utf8_text);

    /**
     * @brief Create sequence from UTF-8 string with language hint
     * @param utf8_text UTF-8 encoded text
     * @param lang Language hint for the text
     * @return RuneSequence created from the text
     */
    static RuneSequence from_utf8(std::string_view utf8_text, language::Code lang);

    // === Comparison operators ===

    [[nodiscard]] bool operator==(const RuneSequence& other) const noexcept {
        return runes_ == other.runes_;
    }

    [[nodiscard]] bool operator!=(const RuneSequence& other) const noexcept {
        return !(*this == other);
    }

    [[nodiscard]] bool operator<(const RuneSequence& other) const noexcept {
        return runes_ < other.runes_;
    }

    [[nodiscard]] bool operator<=(const RuneSequence& other) const noexcept {
        return runes_ <= other.runes_;
    }

    [[nodiscard]] bool operator>(const RuneSequence& other) const noexcept {
        return runes_ > other.runes_;
    }

    [[nodiscard]] bool operator>=(const RuneSequence& other) const noexcept {
        return runes_ >= other.runes_;
    }
};

// === Non-member functions ===

/**
 * @brief Swap two RuneSequences
 */
inline void swap(RuneSequence& lhs, RuneSequence& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace rune_caster