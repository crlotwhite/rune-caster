#include <gtest/gtest.h>
#include "rune_caster/rune.hpp"

using namespace rune_caster;

class RuneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 테스트 설정
    }

    void TearDown() override {
        // 정리
    }
};

TEST_F(RuneTest, BasicConstruction) {
    // ASCII 문자 생성
    Rune ascii_a(U'A');
    EXPECT_EQ(ascii_a.codepoint(), U'A');
    EXPECT_EQ(ascii_a.language(), language::Code::English);

    // 한글 문자 생성
    Rune hangul_ga(U'가');
    EXPECT_EQ(hangul_ga.codepoint(), U'가');
    EXPECT_EQ(hangul_ga.language(), language::Code::Korean);
}

TEST_F(RuneTest, ConstructionWithLanguage) {
    Rune rune(U'A', language::Code::French);
    EXPECT_EQ(rune.codepoint(), U'A');
    EXPECT_EQ(rune.language(), language::Code::French);
}

TEST_F(RuneTest, ConstructionWithPhoneme) {
    Rune rune(U'가', language::Code::Korean, "ga");
    EXPECT_EQ(rune.codepoint(), U'가');
    EXPECT_EQ(rune.language(), language::Code::Korean);
    EXPECT_EQ(rune.phoneme(), "ga");
}

TEST_F(RuneTest, CharacterClassification) {
    // 모음 테스트
    Rune vowel_a(U'a');
    EXPECT_TRUE(vowel_a.is_vowel());
    EXPECT_FALSE(vowel_a.is_consonant());
    EXPECT_TRUE(vowel_a.is_letter());

    // 자음 테스트
    Rune consonant_b(U'b');
    EXPECT_FALSE(consonant_b.is_vowel());
    EXPECT_TRUE(consonant_b.is_consonant());
    EXPECT_TRUE(consonant_b.is_letter());

    // 숫자 테스트
    Rune digit_5(U'5');
    EXPECT_TRUE(digit_5.is_digit());
    EXPECT_FALSE(digit_5.is_letter());

    // 공백 테스트
    Rune space(U' ');
    EXPECT_TRUE(space.is_whitespace());
    EXPECT_FALSE(space.is_letter());

    // 구두점 테스트
    Rune punct(U'!');
    EXPECT_TRUE(punct.is_punctuation());
    EXPECT_FALSE(punct.is_letter());
}

TEST_F(RuneTest, KoreanVowelClassification) {
    // 한글 모음 테스트
    Rune hangul_a(U'ㅏ');
    EXPECT_TRUE(hangul_a.is_vowel());
    EXPECT_FALSE(hangul_a.is_consonant());

    // 한글 자음 테스트 (완성형에서는 자음만으로는 판단 어려움)
    Rune hangul_ga(U'가');
    EXPECT_FALSE(hangul_ga.is_vowel()); // 완성형이므로 모음으로 인식되지 않음
}

TEST_F(RuneTest, UTF8Conversion) {
    // ASCII 문자
    Rune ascii_a(U'A');
    EXPECT_EQ(ascii_a.to_utf8(), "A");

    // 한글 문자
    Rune hangul_ga(U'가');
    std::string utf8_ga = hangul_ga.to_utf8();
    EXPECT_EQ(utf8_ga.size(), 3); // 한글은 UTF-8에서 3바이트
    EXPECT_EQ(utf8_ga, "가");

    // 일본어 히라가나
    Rune hiragana_a(U'あ');
    std::string utf8_a = hiragana_a.to_utf8();
    EXPECT_EQ(utf8_a.size(), 3); // 히라가나도 UTF-8에서 3바이트
    EXPECT_EQ(utf8_a, "あ");
}

TEST_F(RuneTest, UTF16Conversion) {
    // ASCII 문자
    Rune ascii_a(U'A');
    std::u16string utf16_a = ascii_a.to_utf16();
    EXPECT_EQ(utf16_a.size(), 1);
    EXPECT_EQ(utf16_a[0], u'A');

    // 한글 문자 (BMP 내)
    Rune hangul_ga(U'가');
    std::u16string utf16_ga = hangul_ga.to_utf16();
    EXPECT_EQ(utf16_ga.size(), 1);
    EXPECT_EQ(utf16_ga[0], u'가');
}

TEST_F(RuneTest, FactoryFromUTF8) {
    // 단일 ASCII 문자
    Rune ascii_rune = Rune::from_utf8("A");
    EXPECT_EQ(ascii_rune.codepoint(), U'A');
    EXPECT_EQ(ascii_rune.language(), language::Code::English);

    // 한글 문자
    Rune hangul_rune = Rune::from_utf8("가");
    EXPECT_EQ(hangul_rune.codepoint(), U'가');
    EXPECT_EQ(hangul_rune.language(), language::Code::Korean);

    // 언어 힌트와 함께
    Rune rune_with_hint = Rune::from_utf8("A", language::Code::French);
    EXPECT_EQ(rune_with_hint.codepoint(), U'A');
    EXPECT_EQ(rune_with_hint.language(), language::Code::French);
}

TEST_F(RuneTest, FactoryFromUTF8Errors) {
    // 빈 문자열
    EXPECT_THROW(Rune::from_utf8(""), std::invalid_argument);

    // 잘못된 UTF-8 시퀀스
    EXPECT_THROW(Rune::from_utf8("\xFF"), std::invalid_argument);
}

TEST_F(RuneTest, ComparisonOperators) {
    Rune a1(U'A');
    Rune a2(U'A');
    Rune b(U'B');

    // 동등 비교
    EXPECT_TRUE(a1 == a2);
    EXPECT_FALSE(a1 == b);

    // 순서 비교
    EXPECT_TRUE(a1 < b);
    EXPECT_FALSE(b < a1);
}

TEST_F(RuneTest, LanguageDetection) {
    // 한글 자동 감지
    Rune hangul(U'가');
    EXPECT_EQ(hangul.language(), language::Code::Korean);

    // 히라가나 자동 감지
    Rune hiragana(U'あ');
    EXPECT_EQ(hiragana.language(), language::Code::Japanese);

    // 라틴 문자 자동 감지
    Rune latin(U'A');
    EXPECT_EQ(latin.language(), language::Code::English);

    // 한자 자동 감지 (중국어로 기본 설정)
    Rune kanji(U'漢');
    EXPECT_EQ(kanji.language(), language::Code::Chinese);
}

TEST_F(RuneTest, PhonemeMutation) {
    Rune rune(U'가');
    EXPECT_TRUE(rune.phoneme().empty());

    rune.set_phoneme("ga");
    EXPECT_EQ(rune.phoneme(), "ga");

    rune.set_phoneme("ka");
    EXPECT_EQ(rune.phoneme(), "ka");
}

TEST_F(RuneTest, LanguageMutation) {
    Rune rune(U'A');
    EXPECT_EQ(rune.language(), language::Code::English);

    rune.set_language(language::Code::French);
    EXPECT_EQ(rune.language(), language::Code::French);
}
