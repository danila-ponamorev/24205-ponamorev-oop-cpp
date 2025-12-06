#include <gtest/gtest.h>
#include "Application.h"
#include "Config.h"
#include "PresetParser.h"
#include "ConsoleCommandExecutor.h"
#include "ConsoleCommandParser.h"
#include "ConsoleRenderer.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "Grid.h"
#include "PresetLibrary.h"
#include "PresetMaker.h"
#include "Rule.h"
#include <list>



#include <fstream>
#include <filesystem>
#include <sstream>


namespace fs = std::filesystem;

TEST(ConfigTest, DefaultValues) {
    Config config;
    ASSERT_EQ(config.mode, RunMode::DEFAULT);
    ASSERT_TRUE(config.inputFile.empty());
    ASSERT_TRUE(config.outputFile.empty());
    ASSERT_EQ(config.iterations, 0);
}

TEST(ConfigTest, ParseArguments) {
    const char* argv[] = {"program", "input.txt", "-i", "42", "--output=out.txt"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    Config config = Config::parse(argc, const_cast<char**>(argv));
    
    ASSERT_EQ(config.mode, RunMode::OFFLINE);
    ASSERT_EQ(config.inputFile, "input.txt");
    ASSERT_EQ(config.outputFile, "out.txt");
    ASSERT_EQ(config.iterations, 42u);
}

TEST(UniverseTest, CellManipulation) {
    Universe universe;
    universe.setCell(1, 1, CellState::ALIVE);
    
    ASSERT_EQ(universe.getCell(1, 1), CellState::ALIVE);
    ASSERT_EQ(universe.getCell(0, 0), CellState::DEAD);
}

TEST(UniverseTest, Metadata) {
    Universe universe;
    universe.setName("Test");
    universe.setWidth(100);
    universe.setHeight(50);
    
    ASSERT_EQ(universe.getName(), "Test");
    ASSERT_EQ(universe.getWidth(), 100u);
    ASSERT_EQ(universe.getHeight(), 50u);
}

class PresetParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        presetLines = {
            "Life 1.06",
            "#N Test Preset",
            "#R B3/S23",
            "#S 10 20",
            "1 2",
            "3 4",
            "5 6"
        };
    }

    std::list<std::string> presetLines;
};

TEST_F(PresetParserTest, ParsePreset) {
    PresetParser parser;
    parser.parse(presetLines);
    
    ASSERT_EQ(parser.get_name(), "Test Preset");
    ASSERT_EQ(parser.get_width(), 10u);
    ASSERT_EQ(parser.get_height(), 20u);
    
    std::vector<std::pair<int, int>> expectedCells = {{1,2}, {3,4}, {5,6}};
    ASSERT_EQ(parser.get_cells(), expectedCells);
}

TEST(ConsoleCommandExecutorTest, BasicCommands) {
    Universe universe;
    ConsoleCommandExecutor executor(universe);
    
    executor.execute({"tick", "5"});
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


TEST(ConsoleCommandParserTest, EmptyInput) {
    auto tokens = ConsoleCommandParser::parse("");
    ASSERT_TRUE(tokens.empty());
}

TEST(ConsoleCommandParserTest, MultipleTokens) {
    auto tokens = ConsoleCommandParser::parse("tick 5 dump file.txt");
    ASSERT_EQ(tokens.size(), 4);
    ASSERT_EQ(tokens[0], "tick");
    ASSERT_EQ(tokens[1], "5");
    ASSERT_EQ(tokens[2], "dump");
    ASSERT_EQ(tokens[3], "file.txt");
}


// TEST(ConsoleCommandExecutorTest, HelpCommand) {
//     MockUniverse universe;
//     ConsoleCommandExecutor executor(universe);
//     std::stringstream buffer;
//     std::streambuf* prev = std::cout.rdbuf(buffer.rdbuf());
    
//     executor.execute({"help"});
//     std::cout.rdbuf(prev);
    
//     ASSERT_NE(buffer.str().find("Available commands:"), std::string::npos);
// }


// TEST(ConsoleCommandExecutorTest, DumpCommand) {
//     MockUniverse universe;
//     ConsoleCommandExecutor executor(universe);
//     std::stringstream buffer;
//     std::streambuf* prev = std::cout.rdbuf(buffer.rdbuf());
    
//     executor.execute({"dump"});
//     std::cout.rdbuf(prev);
    
//     ASSERT_NE(buffer.str().find("Usage:"), std::string::npos);
// }

TEST(FileReaderTest, ReadAllFromFile) {
    const std::string filename = "testfile.txt";
    std::ofstream outfile(filename);
    outfile << "line1\nline2\nline3";
    outfile.close();

    FileReader reader(filename);
    auto lines = reader.read_all();
    
    ASSERT_EQ(lines.size(), 3);
    ASSERT_EQ(lines.front(), "line1");
    ASSERT_EQ(lines.back(), "line3");
    
    fs::remove(filename);
}

TEST(FileReaderTest, FileNotOpenException) {
    FileReader reader("non_existent_file.xyz");
    ASSERT_THROW(reader.open(), std::runtime_error);
}

TEST(FileWriterTest, WriteToFile) {
    const std::string filename = "output.txt";
    FileWriter writer(filename);
    writer.write_all({"test1", "test2", "test3"});
    
    std::ifstream infile(filename);
    std::string line;
    std::getline(infile, line);
    
    ASSERT_EQ(line, "test1");
    fs::remove(filename);
}

TEST(GridTest, CellOperations) {
    Grid grid(10, 10);
    grid.setCell(2, 3, CellState::ALIVE);
    
    ASSERT_EQ(grid.getCell(2, 3), CellState::ALIVE);
    ASSERT_EQ(grid.getCell(9, 9), CellState::DEAD);
}

TEST(GridTest, ResizeBehavior) {
    Grid grid(5, 5);
    grid.setCell(3, 3, CellState::ALIVE);
    grid.setHeight(3);
    grid.setWidth(3);
    
    ASSERT_EQ(grid.getWidth(), 3);
    ASSERT_EQ(grid.getHeight(), 3);
    ASSERT_EQ(grid.getCell(3, 3), CellState::DEAD);
}

TEST(PresetLibraryTest, GliderCreation) {
    Universe glider;
    glider = PresetLibrary::loadGlider();
    
    ASSERT_EQ(glider.getName(), "Glider");
}

TEST(PresetLibraryTest, GosperGliderGunStructure) {
    Universe gun = PresetLibrary::loadGosperGliderGun();
    
    ASSERT_EQ(gun.getName(), "Gosper Glider Gun");
}

// TEST(PresetParserTest, RuleAndSizeParsing) {
//     std::list<std::string> lines = {
//         "#N Test Rule",
//         "#R B36/S23",
//         "#S 100 200",
//         "1 2",
//         "3 4"
//     };
    
//     PresetParser parser;
//     parser.parse(lines);
    
//     std::cout << "######################SIZEZIZEIZEIZEI: " << std::to_string(parser.get_width()) << std::endl;

//     ASSERT_EQ(parser.get_name(), "Test Rule");
//     ASSERT_EQ(parser.get_rule().toString(), "B36/S23");
//     ASSERT_EQ(parser.get_width(), 100u);
//     ASSERT_EQ(parser.get_height(), 200u);
    
// }

TEST(PresetMakerTest, FormatGeneration) {
    Universe universe("Test", Rule("B3/S23"), 5, 5);
    universe.setCell(2, 2, CellState::ALIVE);
    
    PresetMaker maker;
    auto result = maker.make(universe);
    
    ASSERT_GE(result.size(), 5u);
    ASSERT_EQ(result.front(), "#Life 1.06");
    ASSERT_NE(std::find(result.begin(), result.end(), "#N Test"), result.end());
    ASSERT_NE(std::find(result.begin(), result.end(), "2 2"), result.end());
}

TEST(RuleTest, StandardGameOfLife) {
    Rule rule("B3/S23");
    
    ASSERT_TRUE(rule.shouldBorn(3));
    ASSERT_FALSE(rule.shouldBorn(2));
    ASSERT_TRUE(rule.shouldSurvive(2));
    ASSERT_FALSE(rule.shouldSurvive(4));
}

TEST(RuleTest, InvalidFormatException) {
    ASSERT_THROW(Rule("InvalidRule"), std::invalid_argument);
}

TEST(GridTest, NeighborCounting) {
    Grid grid(5, 5);
    grid.setCell(1, 1, CellState::ALIVE);
    grid.setCell(2, 2, CellState::ALIVE);
    
    ASSERT_EQ(grid.countAliveNeighbors(1, 1), 1);
    ASSERT_EQ(grid.countAliveNeighbors(2, 1), 2);
}

TEST(GridTest, ClearFunctionality) {
    Grid grid(10, 10);
    grid.setCell(5, 5, CellState::ALIVE);
    grid.clear();
    
    ASSERT_EQ(grid.getCell(5, 5), CellState::DEAD);
}

