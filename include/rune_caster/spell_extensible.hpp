#pragma once

#include "spell_base.hpp"
#include <string>

namespace rune_caster {

/**
 * @brief Simplified spell base class
 *
 * Developers inherit from this class when they just want to implement the
 * core transformation logic via `process()` without re-implementing
 * `operator()` or boilerplate metadata accessors.  It keeps full
 * compatibility with the existing `spell_base` / `caster` pipeline and STL
 * functional usage.
 *
 * Example:
 *  class MySpell : public rune_caster::spell_extensible<> {
 *  public:
 *      MySpell() : spell_extensible{"MySpell", "Demo spell"} {}
 *  private:
 *      RuneSequence process(const RuneSequence& in) const override {
 *          // ... transform ...
 *      }
 *  };
 */
template<typename Input = RuneSequence, typename Output = Input>
class spell_extensible : public spell_base<Input, Output> {
public:
    using input_type  = Input;
    using output_type = Output;

    // Construct with spell meta-information.
    explicit spell_extensible(std::string name, std::string desc)
        : name_(std::move(name)), desc_(std::move(desc)) {}

    // Public API expected by spell_base ------------------------------
    output_type operator()(const input_type& input) const override {
        return process(input);
    }
    std::string name() const override { return name_; }
    std::string description() const override { return desc_; }

protected:
    // Derived classes MUST override this with actual logic
    virtual output_type process(const input_type& input) const = 0;

private:
    std::string name_;
    std::string desc_;
};

} // namespace rune_caster
