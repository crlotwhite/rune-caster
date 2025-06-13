#pragma once

#include "spell_base.hpp"
#include <string>
#include <utility>

namespace rune_caster {

// Forward declaration
class RuneSequence;

namespace spell {
namespace composition {

/**
 * @brief Composition of two spells into a single spell pipeline
 *
 * This class allows chaining two spells together, where the output of the first
 * spell becomes the input of the second spell. Uses C++20 concepts for type safety.
 *
 * @tparam FirstSpell The first spell in the composition
 * @tparam SecondSpell The second spell in the composition
 */
template<typename FirstSpell, typename SecondSpell>
class SpellComposition : public sequence_spell {
    static_assert(spell_concept<FirstSpell>, "FirstSpell must satisfy spell_concept");
    static_assert(spell_concept<SecondSpell>, "SecondSpell must satisfy spell_concept");
    static_assert(chainable_spells<FirstSpell, SecondSpell>, "Spells must be chainable");

private:
    FirstSpell first_spell_;
    SecondSpell second_spell_;
    std::string description_;

public:
    using input_type = typename FirstSpell::input_type;
    using output_type = typename SecondSpell::output_type;

    /**
     * @brief Construct a spell composition
     * @param first The first spell to apply
     * @param second The second spell to apply
     */
    constexpr SpellComposition(FirstSpell first, SecondSpell second)
        : first_spell_(std::move(first))
        , second_spell_(std::move(second))
        , description_(first_spell_.description() + " → " + second_spell_.description())
    {
        static_assert(std::same_as<typename FirstSpell::output_type, typename SecondSpell::input_type>,
                     "Output type of first spell must match input type of second spell");
    }

    /**
     * @brief Apply the composed spells
     * @param input The input to transform
     * @return The result of applying both spells in sequence
     */
    output_type operator()(const input_type& input) const override {
        auto intermediate = first_spell_(input);
        return second_spell_(intermediate);
    }

    std::string description() const override {
        return description_;
    }

    std::string name() const override {
        return first_spell_.name() + "→" + second_spell_.name();
    }

    /**
     * @brief Get the first spell in the composition
     */
    const FirstSpell& first_spell() const noexcept { return first_spell_; }

    /**
     * @brief Get the second spell in the composition
     */
    const SecondSpell& second_spell() const noexcept { return second_spell_; }
};

/**
 * @brief Compose two spells into a single spell pipeline
 *
 * This function creates a SpellComposition that applies the first spell,
 * then applies the second spell to the result. Uses C++20 concepts for
 * compile-time validation.
 *
 * @tparam FirstSpell The type of the first spell
 * @tparam SecondSpell The type of the second spell
 * @param first The first spell to apply
 * @param second The second spell to apply
 * @return A composed spell that applies both spells in sequence
 */
template<typename FirstSpell, typename SecondSpell>
auto compose(FirstSpell&& first, SecondSpell&& second) {
    static_assert(spell_concept<FirstSpell>, "FirstSpell must satisfy spell_concept");
    static_assert(spell_concept<SecondSpell>, "SecondSpell must satisfy spell_concept");
    static_assert(chainable_spells<FirstSpell, SecondSpell>, "Spells must be chainable");

    return SpellComposition<std::decay_t<FirstSpell>, std::decay_t<SecondSpell>>{
        std::forward<FirstSpell>(first),
        std::forward<SecondSpell>(second)
    };
}

/**
 * @brief Helper function for casting spells on RuneSequence
 *
 * This is a convenience function that applies a spell to a RuneSequence
 * and returns the transformed result.
 *
 * @tparam Spell The type of spell to apply
 * @param input The input sequence to transform
 * @param spell The spell to apply
 * @return The transformed sequence
 */
template<typename Spell>
RuneSequence cast_spell(const RuneSequence& input, const Spell& spell) {
    static_assert(spell_for<Spell, RuneSequence>, "Spell must accept RuneSequence as input");
    return spell(input);
}

/**
 * @brief Helper function for casting spells on UTF-8 strings
 *
 * This is a convenience function that converts a UTF-8 string to RuneSequence,
 * applies the spell, and converts back to UTF-8 string.
 *
 * @tparam Spell The type of spell to apply
 * @param utf8_input The UTF-8 input string
 * @param spell The spell to apply
 * @return The transformed string as UTF-8
 */
template<typename Spell>
std::string cast_spell(std::string_view utf8_input, const Spell& spell) {
    static_assert(spell_for<Spell, RuneSequence>, "Spell must accept RuneSequence as input");
    static_assert(std::same_as<typename Spell::output_type, RuneSequence>,
                 "Spell must output RuneSequence for string conversion");

    auto sequence = RuneSequence::from_utf8(utf8_input);
    auto result = spell(sequence);
    return result.to_utf8();
}

} // namespace composition

// Import composition utilities into the main spell namespace for convenience
using composition::SpellComposition;
using composition::compose;
using composition::cast_spell;

} // namespace spell
} // namespace rune_caster