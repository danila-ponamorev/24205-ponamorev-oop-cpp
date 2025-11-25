cmake_minimum_required(VERSION 3.10)
project(task-2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    main.cpp
)

add_executable(game_of_life ${SOURCES})
