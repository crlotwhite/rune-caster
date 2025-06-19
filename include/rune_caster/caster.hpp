#pragma once

#include "spell_base.hpp"
#include "rune_sequence.hpp"
#include <type_traits>
#include <utility>
#include <concepts>

namespace rune_caster {

/**
 * @brief A powerful chaining pipeline for applying spells to data
 * 
 * This class enables fluent interface for applying multiple spells in sequence:
 * auto result = make_caster(data).cast(spell1).cast(spell2).result();
 * 
 * Uses C++20 features for perfect forwarding and type deduction.
 * 
 * @tparam T The type of data being processed
 */
template<typename T>
class caster {
private:
    T data_;
    
public:
    using value_type = T;
    
    /**
     * @brief Construct caster with data (C++20 perfect forwarding)
     */
    template<typename U>
    explicit constexpr caster(U&& data) 
        noexcept(std::is_nothrow_constructible_v<T, U>)
        : data_(std::forward<U>(data)) {}
    
    /**
     * @brief Apply a spell and return a new caster with the result (C++20 concepts)
     * 
     * Uses C++20 concepts for compile-time validation and auto return type deduction.
     * The return type is automatically deduced from the spell's output type.
     * 
     * @tparam Spell The spell type to apply
     * @param spell The spell instance to apply
     * @return A new caster with the transformed data
     */
    template<spell_for<T> Spell>
    constexpr auto cast(Spell&& spell) const & -> caster<typename Spell::output_type> {
        return caster<typename Spell::output_type>{spell(data_)};
    }
    
    /**
     * @brief Apply a spell and return a new caster (rvalue reference optimization)
     */
    template<spell_for<T> Spell>
    constexpr auto cast(Spell&& spell) && -> caster<typename Spell::output_type> {
        return caster<typename Spell::output_type>{spell(std::move(data_))};
    }
    
    /**
     * @brief Get the final result (const lvalue reference)
     */
    constexpr const T& result() const & noexcept {
        return data_;
    }
    
    /**
     * @brief Get the final result (rvalue reference - move semantics)
     */
    constexpr T&& result() && noexcept {
        return std::move(data_);
    }
    
    /**
     * @brief Get the final result (const rvalue reference)
     */
    constexpr T result() const && {
        return data_;
    }
    
    /**
     * @brief Access the underlying data directly
     */
    constexpr const T& operator*() const & noexcept {
        return data_;
    }
    
    constexpr T&& operator*() && noexcept {
        return std::move(data_);
    }
    
    /**
     * @brief Check if the caster contains valid data (C++20 concepts)
     */
    constexpr explicit operator bool() const noexcept {
        if constexpr (std::is_pointer_v<T>) {
            return data_ != nullptr;
        } else if constexpr (requires { data_.empty(); }) {
            return !data_.empty();
        } else {
            return true;  // For non-pointer, non-container types are always valid
        }
    }
    
    /**
     * @brief Get type information at compile time
     */
    static constexpr const char* type_name() noexcept {
        return typeid(T).name();
    }
};

/**
 * @brief Factory function to create a caster (C++20 template argument deduction)
 * 
 * This function enables the fluent interface pattern:
 * auto result = make_caster(data).cast(spell1).cast(spell2).result();
 * 
 * @tparam T The type of data (automatically deduced)
 * @param data The data to wrap in a caster
 * @return A caster object ready for spell application
 */
template<typename T>
constexpr auto make_caster(T&& data) noexcept -> caster<std::decay_t<T>> {
    return caster<std::decay_t<T>>{std::forward<T>(data)};
}

/**
 * @brief Pipe operator for functional programming style (C++20 concepts)
 * 
 * Enables: data | spell1 | spell2 | ...
 * This is an alternative to the fluent interface.
 */
template<typename T, spell_concept Spell>
    requires spell_for<Spell, std::decay_t<T>> || 
             (requires { RuneSequence::from_utf8(std::declval<T>()); } && 
              spell_for<Spell, RuneSequence>)
constexpr auto operator|(T&& data, Spell&& spell) {
    if constexpr (spell_for<Spell, std::decay_t<T>>) {
        return make_caster(std::forward<T>(data))
               .cast(std::forward<Spell>(spell))
               .result();
    } else {
        return make_caster(RuneSequence::from_utf8(data))
               .cast(std::forward<Spell>(spell))
               .result();
    }
}

/**
 * @brief Pipe operator for chaining casters (C++20 concepts)
 */
template<typename T, spell_for<T> Spell>
constexpr auto operator|(caster<T>&& caster_obj, Spell&& spell) {
    return std::move(caster_obj).cast(std::forward<Spell>(spell));
}

} // namespace rune_caster 
