#include <gtest/gtest.h>
#include "main_wrapper.h"
#include "word_frequency_analyzer.h"
#include "text_processor.h"
#include <list>

struct TestCase {
    const char* input;
    const char* output;
};

class MainTest : public ::testing::TestWithParam<TestCase> {
};

TEST_P(MainTest, SuccessfulExecution) {
    const TestCase& tc = GetParam();
    char arg0[] = "program";
    char arg1[100];
    char arg2[100];
    strcpy(arg1, tc.input);
    strcpy(arg2, tc.output);
    char* argv[] = {arg0, arg1, arg2, nullptr};
    int argc = 3;
    EXPECT_EQ(main_implementation(argc, argv), 0);
}

INSTANTIATE_TEST_SUITE_P(
    MainTests,
    MainTest,
    ::testing::Values(
        TestCase{"tests/test1_input.txt", "tests/test1_ouput.txt"},
        TestCase{"tests/test2_input.txt", "tests/test2_ouput.txt"},
        TestCase{"tests/test3_input.txt", "tests/test3_ouput.txt"},
        TestCase{"tests/test4_input.txt", "tests/test4_ouput.txt"},
        TestCase{"tests/test5_input.txt", "tests/test5_ouput.txt"}
    )
);

TEST(TextProcessorTest, ExtractWordsBasic) {
    TextProcessor processor;
    std::string text = "Hello world this is a test";
    auto words = processor.extract_words(text);
    
    EXPECT_EQ(words.size(), 6);
    EXPECT_EQ(words.front(), "hello");
    EXPECT_EQ(words.back(), "test");
}

TEST(TextProcessorTest, ExtractWordsWithDelimiters) {
    TextProcessor processor;
    std::string text = "Hello, world! This; is... a test?";
    auto words = processor.extract_words(text);
    
    EXPECT_EQ(words.size(), 6);
    EXPECT_EQ(words.front(), "hello");
    EXPECT_EQ(words.back(), "test");
}

TEST(TextProcessorTest, ExtractWordsCaseSensitivity) {
    TextProcessor processor;
    std::string text = "Hello WORLD MiXeD CaSe";
    auto words = processor.extract_words(text);
    
    // Проверяем, что все слова в нижнем регистре
    for (const auto& word : words) {
        for (char c : word) {
            EXPECT_FALSE(c >= 'A' && c <= 'Z');
        }
    }
}

TEST(TextProcessorTest, ExtractWordsEmpty) {
    TextProcessor processor;
    std::string text = "";
    auto words = processor.extract_words(text);
    
    EXPECT_TRUE(words.empty());
}

TEST(TextProcessorTest, ExtractWordsOnlyDelimiters) {
    TextProcessor processor;
    std::string text = " , . ! ? ; ";
    auto words = processor.extract_words(text);
    
    EXPECT_TRUE(words.empty());
}


TEST(WordFrequencyAnalyzerTest, BasicFrequency) {
    WordFrequencyAnalyzerPrototipe analyzer;
    std::list<std::string> words = {"hello", "world", "hello", "test"};
    
    analyzer.update_frequency(words);
    
    EXPECT_EQ(analyzer.get_total_words(), 4);
    
    auto frequency = analyzer.get_word_frequency();
    EXPECT_EQ(frequency->at("hello"), 2);
    EXPECT_EQ(frequency->at("world"), 1);
    EXPECT_EQ(frequency->at("test"), 1);
}

TEST(WordFrequencyAnalyzerTest, SortFunctionality) {
    WordFrequencyAnalyzerPrototipe analyzer;
    std::list<std::string> words = {"c", "b", "a", "b", "a", "a"}; // a:3, b:2, c:1
    
    analyzer.update_frequency(words);
    analyzer.sort();
    
    auto sorted = analyzer.get_sorted_word_frequency();
    EXPECT_EQ(sorted->front().first, "a");
    EXPECT_EQ(sorted->front().second, 3);
    EXPECT_EQ(sorted->back().first, "c");
    EXPECT_EQ(sorted->back().second, 1);
}

TEST(WordFrequencyAnalyzerTest, EmptyWords) {
    WordFrequencyAnalyzerPrototipe analyzer;
    std::list<std::string> words;
    
    analyzer.update_frequency(words);
    
    EXPECT_EQ(analyzer.get_total_words(), 0);
    EXPECT_TRUE(analyzer.get_word_frequency()->empty());
}

TEST(WordFrequencyAnalyzerTest, SingleWord) {
    WordFrequencyAnalyzerPrototipe analyzer;
    std::list<std::string> words = {"hello"};
    
    analyzer.update_frequency(words);
    
    EXPECT_EQ(analyzer.get_total_words(), 1);
    EXPECT_EQ(analyzer.get_word_frequency()->at("hello"), 1);
}

TEST(WordFrequencyAnalyzerTest, MultipleUpdates) {
    WordFrequencyAnalyzerPrototipe analyzer;
    std::list<std::string> words1 = {"hello", "world"};
    std::list<std::string> words2 = {"hello", "test"};
    
    analyzer.update_frequency(words1);
    analyzer.update_frequency(words2);
    
    EXPECT_EQ(analyzer.get_total_words(), 4);
    EXPECT_EQ(analyzer.get_word_frequency()->at("hello"), 2);
    EXPECT_EQ(analyzer.get_word_frequency()->at("world"), 1);
    EXPECT_EQ(analyzer.get_word_frequency()->at("test"), 1);
}
