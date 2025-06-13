#pragma once

#include "spell_base.hpp"
#include "unicode.hpp"
#include <vector>

namespace rune_caster {

// Forward declaration
class RuneSequence;

namespace spell {
namespace filter {

/**
 * @brief Remove specific character categories using C++20 concepts
 *
 * This spell can either remove characters of specific Unicode categories
 * or keep only characters of specific categories, depending on the mode.
 */
class CategoryFilter : public sequence_spell {
private:
    std::vector<unicode::Category> filtered_categories_;
    bool remove_mode_; // true = remove, false = keep only

public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    /**
     * @brief Construct a CategoryFilter
     * @param categories List of Unicode categories to filter
     * @param remove If true, remove these categories; if false, keep only these categories
     */
    explicit CategoryFilter(std::vector<unicode::Category> categories, bool remove = true);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "CategoryFilter"; }

    /**
     * @brief Get the filtered categories
     * @return Vector of categories being filtered
     */
    const std::vector<unicode::Category>& filtered_categories() const noexcept {
        return filtered_categories_;
    }

    /**
     * @brief Check if this filter is in remove mode
     * @return true if removing specified categories, false if keeping only specified categories
     */
    bool is_remove_mode() const noexcept { return remove_mode_; }
};

/**
 * @brief Remove or keep only specific scripts using C++20 concepts
 *
 * This spell can either remove characters of specific Unicode scripts
 * or keep only characters of specific scripts, depending on the mode.
 */
class ScriptFilter : public sequence_spell {
private:
    std::vector<unicode::Script> filtered_scripts_;
    bool remove_mode_; // true = remove, false = keep only

public:
    using input_type = RuneSequence;
    using output_type = RuneSequence;

    /**
     * @brief Construct a ScriptFilter
     * @param scripts List of Unicode scripts to filter
     * @param remove If true, remove these scripts; if false, keep only these scripts
     */
    explicit ScriptFilter(std::vector<unicode::Script> scripts, bool remove = true);

    RuneSequence operator()(const RuneSequence& input) const override;
    std::string description() const override;
    std::string name() const override { return "ScriptFilter"; }

    /**
     * @brief Get the filtered scripts
     * @return Vector of scripts being filtered
     */
    const std::vector<unicode::Script>& filtered_scripts() const noexcept {
        return filtered_scripts_;
    }

    /**
     * @brief Check if this filter is in remove mode
     * @return true if removing specified scripts, false if keeping only specified scripts
     */
    bool is_remove_mode() const noexcept { return remove_mode_; }
};

} // namespace filter
} // namespace spell
} // namespace rune_caster