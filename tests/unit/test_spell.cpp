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
    auto input = RuneSequence::from_utf8("  Hello   World  ");
    auto result = input | spell::normalize_whitespace();

    EXPECT_EQ(result.to_utf8(), "Hello World");
    EXPECT_EQ(result.size(), 11);
}

TEST_F(SpellTest, WhitespaceNormalizerEmpty) {
    auto input = RuneSequence::from_utf8("");
    auto result = input | spell::normalize_whitespace();

    EXPECT_TRUE(result.empty());
    EXPECT_EQ(result.to_utf8(), "");
}

TEST_F(SpellTest, WhitespaceNormalizerWhitespaceOnly) {
    auto input = RuneSequence::from_utf8("   \t\n   ");
    auto result = input | spell::normalize_whitespace();

    EXPECT_TRUE(result.empty());
    EXPECT_EQ(result.to_utf8(), "");
}

TEST_F(SpellTest, WhitespaceNormalizerNoTrim) {
    auto input = RuneSequence::from_utf8("  Hello   World  ");
    auto result = input | spell::normalize_whitespace(true, false);  // collapse but no trim

    EXPECT_EQ(result.to_utf8(), " Hello World ");
}

TEST_F(SpellTest, WhitespaceNormalizerNoCollapse) {
    auto input = RuneSequence::from_utf8("  Hello   World  ");
    auto result = input | spell::normalize_whitespace(false, true);  // trim but no collapse

    EXPECT_EQ(result.to_utf8(), "Hello   World");
}

TEST_F(SpellTest, WhitespaceNormalizerTabsAndNewlines) {
    auto input = RuneSequence::from_utf8("\t\tHello\n\nWorld\r\n");
    auto result = input | spell::normalize_whitespace();

    EXPECT_EQ(result.to_utf8(), "Hello World");
}

TEST_F(SpellTest, WhitespaceNormalizerMultilingual) {
    auto input = RuneSequence::from_utf8("  안녕하세요   Hello\t\tこんにちは  ");
    auto result = input | spell::normalize_whitespace();

    EXPECT_EQ(result.to_utf8(), "안녕하세요 Hello こんにちは");
}

TEST_F(SpellTest, WhitespaceNormalizerSingleCharacter) {
    auto input = RuneSequence::from_utf8("A");
    auto result = input | spell::normalize_whitespace();

    EXPECT_EQ(result.to_utf8(), "A");
    EXPECT_EQ(result.size(), 1);
}

TEST_F(SpellTest, WhitespaceNormalizerDescription) {
    auto basic_normalizer = spell::normalize_whitespace();
    EXPECT_EQ(basic_normalizer.description(), "Whitespace normalizer (collapse multiple, trim edges)");

    auto collapse_only = spell::normalize_whitespace(true, false);
    EXPECT_EQ(collapse_only.description(), "Whitespace normalizer (collapse multiple)");

    auto trim_only = spell::normalize_whitespace(false, true);
    EXPECT_EQ(trim_only.description(), "Whitespace normalizer (trim edges)");
}

// === CaseConverter Tests ===

TEST_F(SpellTest, CaseConverterLowercase) {
    auto input = RuneSequence::from_utf8("Hello WORLD");
    auto result = input | spell::lowercase();

    auto spell_obj = spell::lowercase();
    EXPECT_EQ(result.to_utf8(), "hello world");
    EXPECT_EQ(spell_obj.description(), "Lowercase Conversion");
}

TEST_F(SpellTest, CaseConverterUppercase) {
    auto input = RuneSequence::from_utf8("Hello world");
    auto result = input | spell::uppercase();

    auto spell_obj = spell::uppercase();
    EXPECT_EQ(result.to_utf8(), "HELLO WORLD");
    EXPECT_EQ(spell_obj.description(), "Uppercase Conversion");
}

TEST_F(SpellTest, CaseConverterTitlecase) {
    auto input = RuneSequence::from_utf8("hello world test");
    auto result = input | spell::titlecase();

    auto spell_obj = spell::titlecase();
    EXPECT_EQ(result.to_utf8(), "Hello World Test");
    EXPECT_EQ(spell_obj.description(), "Titlecase Conversion");
}

TEST_F(SpellTest, CaseConverterEmpty) {
    auto input = RuneSequence::from_utf8("");
    auto result = input | spell::lowercase();

    EXPECT_TRUE(result.empty());
    EXPECT_EQ(result.to_utf8(), "");
}

TEST_F(SpellTest, CaseConverterMultilingual) {
    auto input = RuneSequence::from_utf8("Hello 안녕하세요 こんにちは");
    auto result = input | spell::lowercase();

    // ASCII part should be lowercased, non-ASCII should remain unchanged
    EXPECT_EQ(result.to_utf8(), "hello 안녕하세요 こんにちは");
}

// === 체이닝 테스트 ===

TEST_F(SpellTest, SpellChaining) {
    auto input = RuneSequence::from_utf8("  Hello WORLD  ");
    auto result = input | spell::normalize_whitespace() | spell::lowercase();

    EXPECT_EQ(result.to_utf8(), "hello world");
}

TEST_F(SpellTest, TriplePipeChaining) {
    auto input = RuneSequence::from_utf8("  Hello WORLD  ");
    auto result = input | spell::trim() | spell::lowercase() | spell::uppercase();

    EXPECT_EQ(result.to_utf8(), "HELLO WORLD");
}
