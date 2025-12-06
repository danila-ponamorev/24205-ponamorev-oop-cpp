cmake_minimum_required(VERSION 3.14)
project(test-task-2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(include)

set(SOURCES
    Rule.cpp
    Grid.cpp
    Universe.cpp
    FileReader.cpp
    FileWriter.cpp
    PresetParser.cpp
    PresetMaker.cpp
    ConsoleRenderer.cpp
    PresetLibrary.cpp
    ConsoleCommandParser.cpp
    ConsoleCommandExecutor.cpp
    Config.cpp
    Application.cpp
)

add_executable(game_of_life ${SOURCES} main.cpp)
add_executable(test-task-2 tests.cpp ${SOURCES})

target_link_libraries(test-task-2 
    PRIVATE 
    gtest 
    gtest_main 
)

#target_link_libraries(test-task-2 GTest::GTest GTest::Main)
#add_test(NAME MainTest COMMAND test-task-2)
