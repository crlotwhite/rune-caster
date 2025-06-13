#include <gtest/gtest.h>
#include "rune_caster/spell.hpp"
#include "rune_caster/rune_sequence.hpp"

using namespace rune_caster;

class SpellTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Test setup
    }

    void TearDown() override {
        // Test cleanup
    }
};

// === WhitespaceNormalizer 테스트 ===

TEST_F(SpellTest, WhitespaceNormalizerBasic) {
    spell::WhitespaceNormalizer normalizer;
    
    auto input = RuneSequence::from_utf8("  Hello   World  ");
    auto result = normalizer(input);
    
    EXPECT_EQ(result.to_utf8(), "Hello World");
    EXPECT_EQ(result.size(), 11);
}

TEST_F(SpellTest, WhitespaceNormalizerEmpty) {
    spell::WhitespaceNormalizer normalizer;
    
    auto input = RuneSequence::from_utf8("");
    auto result = normalizer(input);
    
    EXPECT_TRUE(result.empty());
    EXPECT_EQ(result.to_utf8(), "");
}

TEST_F(SpellTest, WhitespaceNormalizerWhitespaceOnly) {
    spell::WhitespaceNormalizer normalizer;
    
    auto input = RuneSequence::from_utf8("   \t\n   ");
    auto result = normalizer(input);
    
    EXPECT_TRUE(result.empty());
    EXPECT_EQ(result.to_utf8(), "");
}

TEST_F(SpellTest, WhitespaceNormalizerNoTrim) {
    spell::WhitespaceNormalizer normalizer(true, false);  // collapse but no trim
    
    auto input = RuneSequence::from_utf8("  Hello   World  ");
    auto result = normalizer(input);
    
    EXPECT_EQ(result.to_utf8(), " Hello World ");
}

TEST_F(SpellTest, WhitespaceNormalizerNoCollapse) {
    spell::WhitespaceNormalizer normalizer(false, true);  // trim but no collapse
    
    auto input = RuneSequence::from_utf8("  Hello   World  ");
    auto result = normalizer(input);
    
    EXPECT_EQ(result.to_utf8(), "Hello   World");
}

TEST_F(SpellTest, WhitespaceNormalizerTabsAndNewlines) {
    spell::WhitespaceNormalizer normalizer;
    
    auto input = RuneSequence::from_utf8("\t\tHello\n\nWorld\r\n");
    auto result = normalizer(input);
    
    EXPECT_EQ(result.to_utf8(), "Hello World");
}

TEST_F(SpellTest, WhitespaceNormalizerMultilingual) {
    spell::WhitespaceNormalizer normalizer;
    
    auto input = RuneSequence::from_utf8("  안녕하세요   Hello\t\tこんにちは  ");
    auto result = normalizer(input);
    
    EXPECT_EQ(result.to_utf8(), "안녕하세요 Hello こんにちは");
}

TEST_F(SpellTest, WhitespaceNormalizerSingleCharacter) {
    spell::WhitespaceNormalizer normalizer;
    
    auto input = RuneSequence::from_utf8("A");
    auto result = normalizer(input);
    
    EXPECT_EQ(result.to_utf8(), "A");
    EXPECT_EQ(result.size(), 1);
}

TEST_F(SpellTest, WhitespaceNormalizerDescription) {
    spell::WhitespaceNormalizer basic_normalizer;
    EXPECT_EQ(basic_normalizer.description(), "Whitespace normalizer (collapse multiple, trim edges)");
    
    spell::WhitespaceNormalizer collapse_only(true, false);
    EXPECT_EQ(collapse_only.description(), "Whitespace normalizer (collapse multiple)");
    
    spell::WhitespaceNormalizer trim_only(false, true);
    EXPECT_EQ(trim_only.description(), "Whitespace normalizer (trim edges)");
}

// === Helper 함수 테스트 ===

TEST_F(SpellTest, CastSpellHelper) {
    spell::WhitespaceNormalizer normalizer;
    
    std::string input = "  Hello   World  ";
    std::string result = spell::cast_spell(input, normalizer);
    
    EXPECT_EQ(result, "Hello World");
}

TEST_F(SpellTest, CastSpellHelperEmpty) {
    spell::WhitespaceNormalizer normalizer;
    
    std::string input = "";
    std::string result = spell::cast_spell(input, normalizer);
    
    EXPECT_EQ(result, "");
} 