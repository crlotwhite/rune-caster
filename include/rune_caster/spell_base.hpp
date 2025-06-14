#pragma once

#include <string>
#include <type_traits>
#include <concepts>
#include <functional>
#include <utility>
#include <ranges>

#include "rune_sequence.hpp"

namespace rune_caster {

// Forward declarations removed to avoid conflict; RuneSequence is defined via alias later.

/**
 * @brief Base interface for all spell algorithms with C++20 enhancements
 *
 * This template class provides a common interface that all spell algorithms
 * must implement, enabling polymorphic behavior and easy extensibility.
 * Uses modern C++20 features for better compile-time checking.
 *
 * @tparam Input Input type for the spell
 * @tparam Output Output type for the spell (defaults to Input)
 */
template<typename Input, typename Output = Input>
class spell_base {
public:
    using input_type = Input;
    using output_type = Output;

    virtual ~spell_base() = default;

    /**
     * @brief Apply the spell transformation
     * @param input The input data to transform
     * @return The transformed output
     */
    virtual output_type operator()(const input_type& input) const = 0;

    /**
     * @brief Get the spell's name
     * @return A string identifying the spell
     */
    virtual std::string name() const = 0;

    /**
     * @brief Get the spell's description
     * @return A human-readable description of what the spell does
     */
    virtual std::string description() const = 0;

    /**
     * @brief Check if this spell can be applied to the given input type (C++20 concepts)
     * @return true if the spell can process the input type
     */
    template<typename T>
    static constexpr bool can_process() noexcept {
        return std::same_as<T, input_type> || std::convertible_to<T, input_type>;
    }

    /**
     * @brief Check if this spell produces the expected output type (C++20 concepts)
     */
    template<typename T>
    static constexpr bool produces() noexcept {
        return std::same_as<T, output_type> || std::convertible_to<output_type, T>;
    }

    /**
     * @brief Get input type name as compile-time string (C++20)
     */
    static constexpr const char* input_type_name() noexcept {
        return typeid(input_type).name();
    }

    /**
     * @brief Get output type name as compile-time string (C++20)
     */
    static constexpr const char* output_type_name() noexcept {
        return typeid(output_type).name();
    }
};

/**
 * @brief Most common spell type: RuneSequence -> RuneSequence
 *
 * This is the standard spell interface that most text processing
 * algorithms will implement.
 */
using sequence_spell = spell_base<RuneSequence>;

// C++20 Concepts - Modern Type Checking

/**
 * @brief Concept defining what makes a valid spell
 */
template<typename T>
concept spell_concept = requires(T spell) {
    typename T::input_type;
    typename T::output_type;
    { spell.name() } -> std::convertible_to<std::string>;
    { spell.description() } -> std::convertible_to<std::string>;
    requires std::invocable<T, typename T::input_type>;
    requires std::same_as<
        std::invoke_result_t<T, typename T::input_type>,
        typename T::output_type
    >;
};

/**
 * @brief Concept for spells that work with specific input types
 */
template<typename Spell, typename Input>
concept spell_for = spell_concept<Spell> &&
                   std::same_as<typename Spell::input_type, Input>;

/**
 * @brief Concept for sequence-based spells (most common case)
 */
template<typename T>
concept sequence_spell_concept = spell_for<T, RuneSequence> &&
                                std::same_as<typename T::output_type, RuneSequence>;

/**
 * @brief Concept for transformation spells that can change the output type
 */
template<typename Spell, typename Input, typename Output>
concept transforming_spell = spell_concept<Spell> &&
                           spell_for<Spell, Input> &&
                           std::same_as<typename Spell::output_type, Output>;

/**
 * @brief Concept for spells that can be chained together
 */
template<typename Spell1, typename Spell2>
concept chainable_spells = spell_concept<Spell1> &&
                          spell_concept<Spell2> &&
                          (std::same_as<typename Spell1::output_type, typename Spell2::input_type> ||
                           std::convertible_to<typename Spell1::output_type, typename Spell2::input_type>);

// Backward compatibility - convert concepts to constexpr bool
template<typename T>
constexpr bool is_spell_v = spell_concept<T>;

template<typename Spell, typename Input>
constexpr bool can_spell_process_v = spell_for<Spell, Input>;

template<typename Spell1, typename Spell2>
constexpr bool chainable_v = chainable_spells<Spell1, Spell2>;

template<typename T>
constexpr bool is_sequence_spell_v = sequence_spell_concept<T>;

} // namespace rune_caster