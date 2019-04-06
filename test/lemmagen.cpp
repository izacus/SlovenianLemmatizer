#include "gtest/gtest.h"
#include "lemmagen.h"

class LemmagenTest : public ::testing::Test {
 protected:
  void SetUp() override {
    lem_load_language_library("../data/lem-me-sl.bin");
  }

  char output_array[256];

  // void TearDown() override {}
};

TEST_F(LemmagenTest, TestHodimoWord) {
    lem_lemmatize_word("hodimo", output_array);
    EXPECT_STREQ("hoditi", output_array);
}

TEST_F(LemmagenTest, TestPlezajoWord) {
    lem_lemmatize_word("plezajo", output_array);
    EXPECT_STREQ("plezati", output_array);
}

TEST_F(LemmagenTest, TestTecevaUTFWord) {
    lem_lemmatize_word("tečeva", output_array);
    EXPECT_STREQ("teči", output_array);
}

TEST_F(LemmagenTest, TestLongSentence) {
    lem_lemmatize_word("Stavek je jezikovna enota, ki je sestavljena iz vec besed, zbranih okoli osebne glagolske oblike oziroma povedka. Je del povedi in osnovni pojem skladnje.", output_array);
    EXPECT_STREQ("Stavek je jezikovna enota, ki je sestavljena iz vec besed, zbranih okoli osebne glagolske oblike oziroma povedka. Je del povedi in osnovni pojem skladnje.", output_array);
}

TEST_F(LemmagenTest, TestAllocatedWork) {
    EXPECT_STREQ("hoditi", lem_lemmatize_word_alloc("hodimo"));
}

TEST_F(LemmagenTest, TestEmptyString) {
    lem_lemmatize_word("", output_array);
    EXPECT_STREQ("", output_array);
}

TEST_F(LemmagenTest, TestAllocatedEmptyString) {
    EXPECT_STREQ("", lem_lemmatize_word_alloc(""));
}

TEST_F(LemmagenTest, TestNullString) {
    lem_lemmatize_word(nullptr, output_array);
    EXPECT_STREQ("", output_array);
}

TEST_F(LemmagenTest, TestNullOutput) {
    lem_lemmatize_word("test", nullptr);
}

TEST_F(LemmagenTest, TestNullAllocatedString) {
    EXPECT_STREQ("", lem_lemmatize_word_alloc(nullptr));
}

