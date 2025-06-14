#include <gtest/gtest.h>
#include "rune_caster/spell.hpp"
#include "rune_caster/rune_sequence.hpp"

using namespace rune_caster;

TEST(TrimEdgesTest, BasicTrim) {
    auto seq = RuneSequence::from_utf8("  Hello World  ");
    spell::TrimEdges trim;
    auto result = trim(seq);
    EXPECT_EQ(result.to_utf8(), "Hello World");
}

TEST(TrimEdgesTest, NoWhitespace) {
    auto seq = RuneSequence::from_utf8("Hello");
    spell::TrimEdges trim;
    auto result = trim(seq);
    EXPECT_EQ(result.to_utf8(), "Hello");
}

TEST(TrimEdgesTest, AllWhitespace) {
    auto seq = RuneSequence::from_utf8("   \t\n  ");
    spell::TrimEdges trim;
    auto result = trim(seq);
    EXPECT_TRUE(result.empty());
}