#include <gtest/gtest.h>
#include "rune_caster/spell.hpp"
#include "rune_caster/rune_sequence.hpp"

using namespace rune_caster;

TEST(TrimEdgesTest, BasicTrim) {
    auto seq = RuneSequence::from_utf8("  Hello World  ");
    auto trim_spell = spell::trim();
    auto result = trim_spell(seq);
    EXPECT_EQ(result.to_utf8(), "Hello World");
}

TEST(TrimEdgesTest, NoWhitespace) {
    auto seq = RuneSequence::from_utf8("Hello");
    auto trim_spell = spell::trim();
    auto result = trim_spell(seq);
    EXPECT_EQ(result.to_utf8(), "Hello");
}

TEST(TrimEdgesTest, AllWhitespace) {
    auto seq = RuneSequence::from_utf8("   \t\n  ");
    auto trim_spell = spell::trim();
    auto result = trim_spell(seq);
    EXPECT_TRUE(result.empty());
}
