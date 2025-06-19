#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <memory>
#include <type_traits>

#include "rune_sequence.hpp"
#include "caster.hpp"

namespace rune_caster {

/**
 * @brief Unified spell object for text transformation
 *
 * Following the Rune design philosophy, this class provides a single,
 * extensible interface for all text transformations. Users can create
 * custom spells using lambdas, functions, or functors while maintaining
 * full compatibility with the Caster pipeline system.
 *
 * Design principles:
 * - Single unified type (like Rune for characters)
 * - Easy extensibility through function objects
 * - Pipeline compatibility with operator|
 * - Zero-overhead abstractions where possible
 * - Full Caster integration
 */
class Spell {
public:
    // === Type definitions ===
    using transform_function = std::function<RuneSequence(const RuneSequence&)>;
    using validation_function = std::function<bool(const RuneSequence&)>;

    // === Constructors ===

    /**
     * @brief Default constructor (identity spell)
     */
    Spell() noexcept;

    /**
     * @brief Construct from transformation function
     * @param transform The transformation function
     * @param name Optional name for the spell
     * @param description Optional description
     */
    explicit Spell(transform_function transform,
                   std::string name = "CustomSpell",
                   std::string description = "User-defined transformation");

    /**
     * @brief Construct from lambda or functor (template)
     * @param func Lambda or functor that transforms RuneSequence
     * @param name Optional name for the spell
     * @param description Optional description
     */
    template<typename Func>
    requires std::invocable<Func, const RuneSequence&> &&
             std::convertible_to<std::invoke_result_t<Func, const RuneSequence&>, RuneSequence>
    explicit Spell(Func&& func,
                   std::string name = "LambdaSpell",
                   std::string description = "Lambda transformation")
        : transform_(std::forward<Func>(func))
        , name_(std::move(name))
        , description_(std::move(description))
        , id_(generate_id()) {}

    // === Copy/Move semantics ===
    Spell(const Spell&) = default;
    Spell(Spell&&) noexcept = default;
    Spell& operator=(const Spell&) = default;
    Spell& operator=(Spell&&) noexcept = default;
    ~Spell() = default;

    // === Core transformation ===

    /**
     * @brief Apply the spell transformation
     * @param input The input sequence to transform
     * @return The transformed sequence
     */
    RuneSequence operator()(const RuneSequence& input) const;

    /**
     * @brief Apply the spell to a UTF-8 string
     * @param utf8_input The UTF-8 input string
     * @return The transformed UTF-8 string
     */
    std::string operator()(std::string_view utf8_input) const;

    // === Pipeline composition ===

    /**
     * @brief Compose with another spell (pipeline operator)
     * @param next The next spell in the pipeline
     * @return A new spell representing the composition
     */
    Spell operator|(const Spell& next) const;

    // Note: Only one operator| definition to avoid ambiguity

    // === Accessors ===

    /**
     * @brief Get the spell name
     * @return The spell name
     */
    [[nodiscard]] const std::string& name() const noexcept { return name_; }

    /**
     * @brief Get the spell description
     * @return The spell description
     */
    [[nodiscard]] const std::string& description() const noexcept { return description_; }

    /**
     * @brief Get the spell unique ID
     * @return The spell ID
     */
    [[nodiscard]] uint64_t id() const noexcept { return id_; }

    // === Validation and properties ===

    /**
     * @brief Check if this spell is the identity transformation
     * @return true if identity, false otherwise
     */
    [[nodiscard]] bool is_identity() const noexcept;

    /**
     * @brief Check if this spell is a composition of multiple spells
     * @return true if composition, false otherwise
     */
    [[nodiscard]] bool is_composition() const noexcept { return is_composition_; }

    /**
     * @brief Get the number of composed spells (1 for simple spells)
     * @return The composition depth
     */
    [[nodiscard]] size_t composition_depth() const noexcept { return composition_depth_; }

    // === Debugging and introspection ===

    /**
     * @brief Get a detailed string representation
     * @return String representation for debugging
     */
    [[nodiscard]] std::string to_string() const;

    /**
     * @brief Test the spell with sample input
     * @param test_input Sample input for testing
     * @return The transformation result
     */
    RuneSequence test(const RuneSequence& test_input) const;

    // === Static factory methods ===

    /**
     * @brief Create an identity spell (no transformation)
     * @return Identity spell
     */
    static Spell identity();

    /**
     * @brief Create a spell that always returns the same result
     * @param constant_result The constant result to return
     * @return Constant spell
     */
    static Spell constant(RuneSequence constant_result);

    /**
     * @brief Create a conditional spell
     * @param condition Condition function
     * @param if_true Spell to apply if condition is true
     * @param if_false Spell to apply if condition is false
     * @return Conditional spell
     */
    static Spell conditional(validation_function condition,
                           const Spell& if_true,
                           const Spell& if_false);

private:
    transform_function transform_;     ///< The transformation function
    std::string name_;                ///< Spell name
    std::string description_;         ///< Spell description
    uint64_t id_;                    ///< Unique identifier
    bool is_composition_;            ///< True if this is a composition
    size_t composition_depth_;       ///< Depth of composition

    /**
     * @brief Generate a unique ID for the spell
     * @return Unique ID
     */
    static uint64_t generate_id();
};

// === Factory functions for common spells ===

namespace spell_factory {

/**
 * @brief Create a spell from a lambda
 * @param lambda The lambda function
 * @param name Optional name
 * @param description Optional description
 * @return The spell object
 */
template<typename Lambda>
auto make_spell(Lambda&& lambda,
                std::string name = "CustomSpell",
                std::string description = "Custom transformation") {
    return Spell{std::forward<Lambda>(lambda), std::move(name), std::move(description)};
}

/**
 * @brief Create a spell that applies a transformation to each rune
 * @param rune_transform Function that transforms individual runes
 * @param name Optional name
 * @param description Optional description
 * @return The spell object
 */
template<typename RuneTransform>
auto per_rune_spell(RuneTransform&& rune_transform,
                   std::string name = "PerRuneSpell",
                   std::string description = "Per-rune transformation") {
    return make_spell([transform = std::forward<RuneTransform>(rune_transform)]
                     (const RuneSequence& input) -> RuneSequence {
        RuneSequence result;
        for (const auto& rune : input) {
            result.append(transform(rune));
        }
        return result;
    }, std::move(name), std::move(description));
}

/**
 * @brief Create a spell that filters runes based on a predicate
 * @param predicate Function that returns true for runes to keep
 * @param name Optional name
 * @param description Optional description
 * @return The spell object
 */
template<typename Predicate>
auto filter_spell(Predicate&& predicate,
                 std::string name = "FilterSpell",
                 std::string description = "Rune filtering") {
    return make_spell([pred = std::forward<Predicate>(predicate)]
                     (const RuneSequence& input) -> RuneSequence {
        RuneSequence result;
        for (const auto& rune : input) {
            if (pred(rune)) {
                result.append(rune);
            }
        }
        return result;
    }, std::move(name), std::move(description));
}

} // namespace spell_factory

// === Integration with Caster ===
// Note: Caster integration will be handled in caster.hpp

} // namespace rune_caster

// === User-defined literals ===

namespace rune_caster::literals {

/**
 * @brief Create a simple string replacement spell
 * Usage: auto spell = "old_text -> new_text"_spell;
 */
Spell operator""_spell(const char* replacement_rule, size_t len);

} // namespace rune_caster::literals
