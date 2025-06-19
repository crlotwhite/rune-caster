#include "spell_unified.hpp"
#include "rune_sequence.hpp"
#include <atomic>
#include <sstream>

namespace rune_caster {

// === Static ID generation ===

uint64_t Spell::generate_id() {
    static std::atomic<uint64_t> next_id{1};
    return next_id.fetch_add(1);
}

// === Constructors ===

Spell::Spell() noexcept
    : transform_([](const RuneSequence& input) { return input; })
    , name_("Identity")
    , description_("Identity transformation (no change)")
    , id_(generate_id())
    , is_composition_(false)
    , composition_depth_(1) {
}

Spell::Spell(transform_function transform,
             std::string name,
             std::string description)
    : transform_(std::move(transform))
    , name_(std::move(name))
    , description_(std::move(description))
    , id_(generate_id())
    , is_composition_(false)
    , composition_depth_(1) {
}

// === Core transformation ===

RuneSequence Spell::operator()(const RuneSequence& input) const {
    return transform_(input);
}

std::string Spell::operator()(std::string_view utf8_input) const {
    auto sequence = RuneSequence::from_utf8(utf8_input);
    auto result = transform_(sequence);
    return result.to_utf8();
}

// === Pipeline composition ===

Spell Spell::operator|(const Spell& next) const {
    auto composed_transform = [this_transform = transform_, next_transform = next.transform_]
                             (const RuneSequence& input) -> RuneSequence {
        auto intermediate = this_transform(input);
        return next_transform(intermediate);
    };

    std::string composed_name = name_ + "|" + next.name_;
    std::string composed_description = description_ + " → " + next.description_;

    Spell result(composed_transform, composed_name, composed_description);
    result.is_composition_ = true;
    result.composition_depth_ = composition_depth_ + next.composition_depth_;

    return result;
}

// === Validation and properties ===

bool Spell::is_identity() const noexcept {
    return name_ == "Identity" && !is_composition_;
}

// === Debugging and introspection ===

std::string Spell::to_string() const {
    std::ostringstream oss;
    oss << "Spell{";
    oss << "id=" << id_;
    oss << ", name=\"" << name_ << "\"";
    oss << ", description=\"" << description_ << "\"";
    oss << ", composition=" << (is_composition_ ? "true" : "false");
    oss << ", depth=" << composition_depth_;
    oss << "}";
    return oss.str();
}

RuneSequence Spell::test(const RuneSequence& test_input) const {
    return transform_(test_input);
}

// === Static factory methods ===

Spell Spell::identity() {
    return Spell{}; // Default constructor creates identity
}

Spell Spell::constant(RuneSequence constant_result) {
    auto transform = [result = std::move(constant_result)]
                    (const RuneSequence&) -> RuneSequence {
        return result;
    };

    return Spell(transform, "Constant", "Always returns the same result");
}

Spell Spell::conditional(validation_function condition,
                        const Spell& if_true,
                        const Spell& if_false) {
    auto transform = [condition, if_true, if_false]
                    (const RuneSequence& input) -> RuneSequence {
        if (condition(input)) {
            return if_true(input);
        } else {
            return if_false(input);
        }
    };

    std::string name = "Conditional(" + if_true.name() + "/" + if_false.name() + ")";
    std::string description = "Conditional: " + if_true.description() + " OR " + if_false.description();

    return Spell(transform, name, description);
}

} // namespace rune_caster

// === User-defined literals ===

namespace rune_caster::literals {

Spell operator""_spell(const char* replacement_rule, size_t len) {
    std::string rule(replacement_rule, len);

    // Simple parser for "old -> new" format
    auto arrow_pos = rule.find(" -> ");
    if (arrow_pos == std::string::npos) {
        // Just return identity if format is wrong
        return Spell::identity();
    }

    std::string old_text = rule.substr(0, arrow_pos);
    std::string new_text = rule.substr(arrow_pos + 4);

    auto transform = [old_text, new_text](const RuneSequence& input) -> RuneSequence {
        auto utf8_input = input.to_utf8();

        // Simple string replacement
        std::string result = utf8_input;
        size_t pos = 0;
        while ((pos = result.find(old_text, pos)) != std::string::npos) {
            result.replace(pos, old_text.length(), new_text);
            pos += new_text.length();
        }

        return RuneSequence::from_utf8(result);
    };

    std::string name = "Replace(\"" + old_text + "\" -> \"" + new_text + "\")";
    std::string description = "Replace \"" + old_text + "\" with \"" + new_text + "\"";

    return Spell(transform, name, description);
}

} // namespace rune_caster::literals
